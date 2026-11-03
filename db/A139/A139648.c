/*
 * A139648  Primes of the form x^2 + 165*y^2.
 * https://oeis.org/A139648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9754 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation returns the n‑th prime of the form x² + 165·y².
 * It scans primes congruent to the possible residues modulo 660
 * (1,49,169,181,229,289,301,361,421,529) and checks whether
 * a representation exists by a simple loop over y.  The function
 * uses the helper library functions isPrimeGeneric, floor_sqrt
 * and append_candidate.  A static cache allows arbitrary order
 * of calls to a. */
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* --------------------------------------------------------------------- */

static bool representable(seq_t p)
{
    /* Search for y such that p - 165*y² is a perfect square. */
    for (seq_t y = 1; ; ++y)
    {
        seq_t term = (seq_t)165 * y * y;  /* 165*y² */
        if (term > p) break;
        seq_t rem = p - term;
        seq_t x   = floor_sqrt(rem);
        if (x * x == rem) return true;
    }
    return false;
}

/* ------------------------------------------------------------ */

static seq_t *prime_cache = NULL;  /* dynamic array of found primes */
static size_t cache_len   = 0;
static size_t cache_cap   = 0;

/* Residues allowed by the theory (mod 660).  Only the admissible
 * residues are inspected because other residues cannot produce a
 * prime of the required shape. */
static const unsigned int residues[10] = {
    1, 49, 169, 181, 229, 289, 301, 361, 421, 529
};
static const size_t residues_len = 10;

/* Current position while generating new candidates. */
static idx_t  cand_k   = 0;   /* multiple of 660 */
static size_t cand_r_i = 0;   /* index into residues array */

/* Generate terms until at least n primes are cached. */
static void generate_terms(idx_t n)
{
    /* limit the candidate value to the maximal representable seq_t */
    seq_t max_seq_t = (seq_t)-1;
    idx_t max_idx   = (idx_t)max_seq_t;

    while (cache_len < (size_t)n)
    {
        idx_t candidate_idx = residues[cand_r_i] + (idx_t)660 * cand_k;

        if (candidate_idx > max_idx)    /* cannot represent any more values */
            return;

        seq_t candidate = (seq_t)candidate_idx;

        /* Skip numbers before the first term of the sequence. */
        if (candidate < 181)
        {
            cand_r_i++;
            if (cand_r_i == residues_len)
            {
                cand_r_i = 0;
                cand_k++;
            }
            continue;
        }

        if (isPrimeGeneric(candidate) && representable(candidate))
            append_candidate(&prime_cache, &cache_len, &cache_cap, candidate);

        /* Advance to next residue. */
        cand_r_i++;
        if (cand_r_i == residues_len)
        {
            cand_r_i = 0;
            cand_k++;
        }
    }
}

/* Public interface. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */
    generate_terms(n);
    return prime_cache[(size_t)(n - 1)];
}
