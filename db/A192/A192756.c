/*
 * A192756  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4) with a(0)=0,
 *            a(1)=1, a(2)=6, a(3)=17 (derived from the generating function in
 *            the OEIS entry)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;   /* highest index whose value is stored */

    /* initialise cache with the first four terms if not yet done */
    if (cache == NULL) {
        idx_t init_size = 4;
        if (init_size < n + 1) init_size = n + 1;
        cache = (seq_t*)malloc(init_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed – fallback */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)17;
        cached_n = 3;
    }

    /* enlarge cache if necessary */
    if (n > cached_n) {
        if (n + 1 > cached_n + 1) {
            idx_t new_size = n + 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failed – fallback */
            cache = tmp;
        }

        /* compute missing terms using the recurrence */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            seq_t t4 = cache[i - 4];
            cache[i] = (seq_t)3 * t1 - (seq_t)2 * t2 - t3 + t4;
        }
        cached_n = n;
    }

    return cache[n];
}
