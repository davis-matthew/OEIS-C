/*
 * A091686  a(n) = 0^n + ((n-9)/9)*(1-10^n).
 * https://oeis.org/A091686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((n-9)*(1-10^n))/9 (0^0 = 1, 0^n = 0 for n>0)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 18, i128 n <= 36,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 903 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* for malloc, realloc */
#include "../utils/mem.h"

static seq_t *pow10_cache = NULL; /* cached powers of 10, pow10_cache[i] = 10^i */
static idx_t  pow10_cache_size = 0;  /* number of cached elements */

/* Ensure the cache contains 10^k for all k <= n */
static void ensure_pow10(idx_t n)
{
    if (pow10_cache == NULL) {
        /* initial allocation for 10^0 */
        pow10_cache = mem_new(seq_t, 1);
        if (!pow10_cache) return;               /* out of memory – let the caller handle it */
        pow10_cache[0] = (seq_t)1;
        pow10_cache_size = 1;
    }

    while (pow10_cache_size <= n) {
        idx_t   old_size = pow10_cache_size;
        idx_t   new_size = old_size * 2;
        if (new_size <= n) new_size = n + 1;

        seq_t *tmp = mem_resize(seq_t, pow10_cache, new_size);
        if (!tmp) return;  /* out of memory – abort and let caller handle it */
        pow10_cache = tmp;

        /* Compute new entries */
        for (idx_t i = old_size; i < new_size; ++i)
            pow10_cache[i] = pow10_cache[i - 1] * (seq_t)10;

        pow10_cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    /* 0^0 = 1 ; 0^n = 0 for n>0 */
    if (n == 0)
        return (seq_t)1;

    ensure_pow10(n);

    /* Calculate (1 - 10^n) and (n - 9) */
    seq_t pow10        = pow10_cache[n];            /* 10^n */
    seq_t pow_minus_one = pow10 - (seq_t)1;          /* 1 - 10^n  (negative for n>0) */
    seq_t n_minus_9   = (seq_t)n - (seq_t)9;

    /* (n-9)*(1-10^n) / 9 is guaranteed to be an integer */
    seq_t prod = (pow_minus_one * n_minus_9) / (seq_t)9;

    return -prod;   /* original formula uses (1-10^n), not (10^n-1) */
}
