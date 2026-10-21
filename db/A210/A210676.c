/*
 * A210676  a(0)=1; thereafter a(n) = -3*Sum_{k=1..n} binomial(2n,2k)*a(n-k).
 * https://oeis.org/A210676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence A210676
   Recurrence:
       a(0) = 1
       a(n) = -3 * Σ_{k=1..n} binom(2n, 2k) * a(n-k)  (n ≥ 1)
   This implementation uses a static cache so that calls may be made
   in any order.  The cache is grown only when needed.  All arithmetic
   is performed with the build‑dependent type seq_t.  No floating point
   operations are used. */

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) once it has been computed. */
    static seq_t *cache = NULL;      /* pointer to array of seq_t */
    static size_t cache_cap = 0;     /* number of slots currently allocated */

    /* Remember the previous capacity before any growth. */
    size_t old_cap = cache_cap;

    /* Grow the cache so that indices up to n are valid. */
    if ((size_t)n >= old_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed: return zero as a fallback. */
            return (seq_t)0;
        }
        cache = tmp;
        /* Initialise the newly allocated slots to zero. */
        for (size_t i = old_cap; i < new_cap; ++i) {
            cache[i] = (seq_t)0;
        }
        cache_cap = new_cap;
    }

    /* Ensure the base case a(0)=1 is set. */
    if (cache[0] == (seq_t)0) {
        cache[0] = (seq_t)1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = (idx_t)old_cap; i <= n; ++i) {
        if (i == 0) continue;            /* already handled */

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;           /* binomial(2*i,0) */

        seq_t two_i = (seq_t)(2 * i);

        for (idx_t k = 1; k <= i; ++k) {
            /* Update binom to C(2*i, 2*k) using a multiplicative formula
               to avoid large intermediate products. */
            seq_t mult1 = two_i - (seq_t)(2 * k) + (seq_t)2;   /* (2i-2k+2) */
            seq_t div1  = (seq_t)(2 * k) - (seq_t)1;           /* (2k-1)   */
            seq_t mult2 = two_i - (seq_t)(2 * k) + (seq_t)1;   /* (2i-2k+1) */
            seq_t div2  = (seq_t)(2 * k);                     /* (2k)     */

            binom = binom * mult1;
            binom = binom / div1;
            binom = binom * mult2;
            binom = binom / div2;

            sum += binom * cache[(size_t)(i - k)];
        }

        /* a(i) = -3 * sum */
        cache[(size_t)i] = sum * (seq_t)(-3);
    }

    return cache[(size_t)n];
}
