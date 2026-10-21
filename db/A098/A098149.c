/*
 * A098149  a(0)=-1, a(1)=-1, a(n)=-3*a(n-1)-a(n-2) for n>1.
 * https://oeis.org/A098149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -3*a(n-1)-a(n-2), a(0) = -1, a(1) = -1
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 22, i64 n <= 45, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[i] = a(i) */
static idx_t cache_len = 0;      /* current number of cached elements */

seq_t a(idx_t n)
{
    /* Allocate/extend cache if needed */
    if (cache_len <= n) {
        size_t newsize = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) {
            /* memory failure – return a safe fallback value */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0 || i == 1) {
                cache[i] = (seq_t)-1;
            } else {
                seq_t t = (seq_t)3 * cache[i-1];
                cache[i] = -t - cache[i-2];
            }
        }
        cache_len = newsize;
    }
    return cache[n];
}
