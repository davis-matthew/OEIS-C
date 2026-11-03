/*
 * A154370  Numbers k such that gpf(composite(k)) - lpf(composite(k)) is
 *          prime.
 * https://oeis.org/A154370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 8501, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9582 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, largest_prime_factor, mem, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A154370.
   The function generates terms on demand: it keeps a cache of terms
   and iterates over composite numbers, checking whether
   gpf(composite) – lpf(composite) is prime.  The k-th term is the
   index of the composite number that satisfies this property. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/largest_prime_factor.h"
#include "../utils/smallest_prime_factor.h"
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

/* Ensure the terms array has room for at least 'needed' entries. */
static void
ensure_terms_cap(size_t needed, seq_t **arr, size_t *cap)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
        if (!tmp)
            exit(1); /* unrecoverable; should not happen in tests */
        *arr   = tmp;
        *cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 0 */

    /* static state keeps the sequence memoized in any call order */
    static seq_t *terms = NULL;
    static size_t   terms_len   = 0;
    static size_t   terms_cap   = 0;
    static idx_t    next_num    = 4;      /* first composite number   */
    static idx_t    next_comp_idx = 1;    /* index of next_num        */

    /* generate terms until we have at least 'n' of them */
    while (terms_len < n) {
        /* test if next_num is composite */
        uint64_t sp = smallest_prime_factor((uint64_t)next_num);
        if (sp != next_num) {                 /* composite */
            uint64_t lp   = largest_prime_factor((uint64_t)next_num);
            uint64_t diff = (lp > sp) ? (lp - sp) : (sp - lp);

            /* diff > 0 for a composite; check primality */
            if (diff > 0 && isPrimeGeneric((seq_t)diff)) {
                ensure_terms_cap(terms_len + 1, &terms, &terms_cap);
                terms[terms_len++] = (seq_t)next_comp_idx;
            }
            next_comp_idx++;                  /* advance only for composites */
        }
        next_num++;                            /* try next integer */
    }

    return terms[n - 1];
}
