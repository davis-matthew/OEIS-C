/*
 * A063300  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          27 ).
 * https://oeis.org/A063300
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1)+a(n-3)-a(n-4) a(2) = -1, a(3) = 13, a(4) = 35,
 *            a(5) = 56, a(6) = 77
 * Range:     i8 n <= 8, i16 n <= 51, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* allocated size of cache */
static idx_t cache_last = 1;  /* last index known (=1 initially) */

seq_t a(idx_t n)
{
    if (n < 2)         /* undefined for n < 2 – return 0 */
        return 0;

    /* Grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;                 /* need indices 0..n inclusive */
        size_t sz = (size_t) new_cap * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, sz);
        if (!tmp) return 0;   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values that have not yet been cached */
    idx_t start = cache_last + 1;
    if (start < 2) start = 2;

    for (idx_t i = start; i <= n; ++i) {
        switch (i) {
            case 2:  cache[2] = -1;          break;
            case 3:  cache[3] = 13;          break;
            case 4:  cache[4] = 35;          break;
            case 5:  cache[5] = 56;          break;
            case 6:  cache[6] = 77;          break;
            default: /* recurrence for n > 6 */
                cache[i] = cache[i-1] + cache[i-3] - cache[i-4];
                break;
        }
    }
    cache_last = n;
    return cache[n];
}
