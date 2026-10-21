/*
 * A077993  Expansion of 1/(1+2*x+2*x^2+2*x^3).
 * https://oeis.org/A077993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -2*a(n-1) - 2*a(n-2) - 2*a(n-3) with initial values
 *            a(0)=1, a(1)=-2, a(2)=2
 * Range:     i8 n <= 11, i16 n <= 24, i32 n <= 50, i64 n <= 101,
 *            i128 n <= 203, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache      = NULL;
    static idx_t  cap        = 0;   /* allocated capacity */
    static idx_t  computed   = 0;   /* number of values computed (len) */

    /* ensure capacity at least n+1, and at least 3 for the initial values */
    if (cap < n + 1) {
        idx_t new_cap = n + 1;
        if (new_cap < 3) new_cap = 3;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (cache && computed > 0) {
            memcpy(new_cache, cache, computed * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* initialise the first three terms on the very first call */
    if (computed == 0) {
        cache[0] = 1;          /* a(0) */
        cache[1] = -2;         /* a(1) */
        cache[2] = 2;          /* a(2) */
        computed = 3;
    }

    /* compute any missing terms up to n */
    if (n >= computed) {
        for (idx_t i = computed; i <= n; ++i) {
            seq_t val = -2 * cache[i-1];
            val -= 2 * cache[i-2];
            val -= 2 * cache[i-3];
            cache[i] = val;
        }
        computed = n + 1;
    }

    return cache[n];
}
