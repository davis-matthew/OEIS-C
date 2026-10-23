/*
 * A153732  Binomial transform of A109747.
 * https://oeis.org/A153732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-1) - Sum_{k=1..n} (-1)^k * binomial(n-1,k-1) *
 *            a(n-k) with the initial value a(0) = 1. This is the binomial
 *            transform of A109747
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 21, i64 n <= 31, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;          /* number of elements allocated */

    /* if the requested index is already computed, return it */
    if (n < cap) {
        return cache[n];
    }

    /* grow the cache to accommodate index n */
    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();           /* allocation failed */
    cache = new_cache;

    /* initialise a(0) on first use */
    if (cap == 0) cache[0] = (seq_t)1;

    /* compute all new entries up to n */
    for (idx_t i = cap; i <= n; ++i) {
        if (i == 0) {                 /* safety – already set above */
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum   = (seq_t)0;        /* the summation term */
        seq_t binom = (seq_t)1;        /* binomial(i-1,0) */

        for (idx_t j = 1; j <= i; ++j) {
            seq_t term = binom * cache[i - j];
            /* add or subtract according to (-1)^j */
            if (j & 1U) sum -= term;  /* odd j → negative */
            else        sum += term;  /* even j → positive */

            /* update binom for next j (if any) */
            if (j < i) {
                seq_t mult = (seq_t)(i - j);          /* (i-1) - (j-1) = i-j */
                seq_t div  = (seq_t)j;                 /* division exact */
                binom = binom * mult / div;
            }
        }

        seq_t two   = (seq_t)2;
        cache[i] = two * cache[i - 1] - sum;
    }

    cap = new_cap;
    return cache[n];
}
