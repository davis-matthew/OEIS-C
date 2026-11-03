/*
 * A198047  Primes remaining primes under map 3<=>5 (interchange of decimal
 *          digits 3 and 5).
 * https://oeis.org/A198047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A198047 – primes p containing at least one digit 3 and one digit 5,
   such that the number obtained by swapping all 3↔5 digits is also prime.
   The function scans integers in increasing order, checks the
   digit condition and primality of both numbers, and caches the
   results for fast repeated access. */

/* Include necessary headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* seq_t and idx_t are supplied by the build. */

/* Dynamic array for cached terms. */
static seq_t *terms   = NULL;   /* pointer to array of seq_t values */
static size_t terms_len = 0;    /* number of terms cached */
static size_t terms_cap = 0;    /* allocated capacity */

/* Next integer to examine (stored as idx_t to avoid overflow). */
static idx_t next_cand = 2;

static void ensure_terms_capacity(size_t needed)
{
    if (terms_cap >= needed)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_ptr = mem_resize(seq_t, terms, new_cap);
    if (!new_ptr)
        exit(1);   /* out of memory – terminate */
    terms = new_ptr;
    terms_cap = new_cap;
}

/* Append a new prime to the cache. */
static void append_term(seq_t val)
{
    ensure_terms_capacity(terms_len + 1);
    terms[terms_len++] = val;
}

/* Return the nth term (1‑based).  Results are memoised. */
seq_t a(idx_t n)
{
    /* Guard against n == 0 – behaviour is undefined per OEIS spec. */
    if (n == 0)
        return 0;

    /* Ensure we have at least n terms cached. */
    while (terms_len < (size_t)n)
    {
        /* Scan the next candidate number. */
        idx_t cand = next_cand++;
        if (cand < 10)   /* cannot have both 3 and 5 if <10 */
            continue;

        /* Compute the swapped‑digit number and check that
           the candidate contains at least one 3 and one 5. */
        idx_t tmp = cand;
        idx_t swapped = 0;
        idx_t pow10   = 1;
        bool has3 = false, has5 = false;

        while (tmp > 0)
        {
            int d = (int)(tmp % 10);
            if (d == 3) has3 = true;
            if (d == 5) has5 = true;

            int nd;
            if (d == 3)       nd = 5;
            else if (d == 5)  nd = 3;
            else              nd = d;

            swapped += (idx_t)nd * pow10;
            pow10   *= 10;
            tmp      = tmp / 10;
        }

        if (!has3 || !has5)
            continue;   /* candidate does not contain both digits */

        /* Primality tests – cast to seq_t for the helper function. */
        if (!is_prime_big((seq_t)cand))
            continue;
        if (!is_prime_big((seq_t)swapped))
            continue;

        /* Candidate satisfies all conditions – cache it. */
        append_term((seq_t)cand);
    }

    return terms[(size_t)n - 1];
}
