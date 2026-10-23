/*
 * A020927  Expansion of (1-4*x)^(15/2).
 * https://oeis.org/A020927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-4)^n * binomial(15/2, n). Recurrence:
 *            n*a(n)+2*(-2*n+17)*a(n-1)=0, a(0)=1
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 26, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache[i] holds a(i) */
    static idx_t  cache_cap = 0;    /* allocated size of cache */
    static idx_t  last_done = 0;    /* largest index computed so far */

    /* initialise cache for a(0) on first call */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        cache[0] = 1;                  /* a(0)=1 */
        last_done = 0;
    }

    /* grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = last_done + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t temp = (seq_t)i;          /* i as seq_t */
        temp = temp * 2 - 17;           /* 2*i - 17 */
        seq_t factor = temp * 2;        /* 2*(2*i - 17) */
        seq_t num = prev * factor;      /* prev * factor */
        cache[i] = num / (seq_t)i;      /* integer division by i */
    }

    last_done = n;
    return cache[n];
}
