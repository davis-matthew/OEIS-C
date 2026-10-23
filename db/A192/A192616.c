/*
 * A192616  Constant term in the reduction of the n-th Fibonacci polynomial by
 *          x^3->x^2+x+1. See Comments.
 * https://oeis.org/A192616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+4*a(n-2)-a(n-3)-4*a(n-4)+a(n-5)+a(n-6) Initial
 *            values (offset 1): a(1)=1, a(2)=0, a(3)=1, a(4)=1, a(5)=2,
 *            a(6)=6
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset 1; below offset undefined */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;                 /* largest n already computed */

    /* enlarge cache if necessary */
    if (n > cached_n) {
        size_t new_size = (size_t)(n + 1);     /* +1 to use 1‑based indexing */
        seq_t *new_cache = (seq_t*)malloc(sizeof(seq_t) * new_size);
        if (cache && cached_n > 0)
            memcpy(new_cache, cache, sizeof(seq_t) * (size_t)(cached_n + 1));

        /* initialise base cases up to n if not already set */
        if (new_cache) {
            if (n >= 1) new_cache[1] = 1;
            if (n >= 2) new_cache[2] = 0;
            if (n >= 3) new_cache[3] = 1;
            if (n >= 4) new_cache[4] = 1;
            if (n >= 5) new_cache[5] = 2;
            if (n >= 6) new_cache[6] = 6;
        }

        /* free old cache */
        free(cache);
        cache = new_cache;

        /* compute missing terms using the recurrence */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i <= 6) continue;             /* base terms already set */

            /* a(i) = a(i-1)+4*a(i-2)-a(i-3)-4*a(i-4)+a(i-5)+a(i-6) */
            seq_t v = cache[i-1] + (cache[i-2] << 2);   /* 4*a(i-2) */
            v -= cache[i-3];
            v -= (cache[i-4] << 2);           /* 4*a(i-4) */
            v += cache[i-5];
            v += cache[i-6];
            cache[i] = v;
        }

        cached_n = n;
    }

    return cache[n];
}
