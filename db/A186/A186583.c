/*
 * A186583  Number of (n+3) X 6 0..2 arrays with every 4 X 4 subblock
 *          commuting with each horizontal and vertical neighbor 4 X 4
 *          subblock.
 * https://oeis.org/A186583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-4) - 4*a(n-5) + 2*a(n-6) +
 *            2*a(n-8) - 2*a(n-9) for n>13 Initial values: a(1)=38608,
 *            a(2)=1991, a(3)=5879, a(4)=39757, a(5)=115699, a(6)=149305,
 *            a(7)=156259, a(8)=160945, a(9)=165739, a(10)=171145,
 *            a(11)=185131, a(12)=230017, a(13)=470139
 * Range:     u8 none, u16 n <= 4, u32 n <= 39, u64 n <= 105, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed values */
    static seq_t *cache   = NULL;
    static idx_t  cache_last = 0;   /* largest index computed so far */
    static size_t  cache_cap  = 0;   /* allocated size */

    /* allocate and initialise first time */
    if (cache == NULL) {
        cache_cap   = 14;                      /* indices 0..13, index 0 unused   */
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < cache_cap; ++i) cache[i] = 0;

        cache[1] = 38608;
        cache[2] = 1991;
        cache[3] = 5879;
        cache[4] = 39757;
        cache[5] = 115699;
        cache[6] = 149305;
        cache[7] = 156259;
        cache[8] = 160945;
        cache[9] = 165739;
        cache[10] = 171145;
        cache[11] = 185131;
        cache[12] = 230017;
        cache[13] = 470139;

        cache_last = 13;
    }

    /* extend cache if necessary */
    if (n > cache_last) {
        /* ensure capacity */
        if (n + 1 > cache_cap) {
            size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 1;
            if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();            /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            if (i <= 13) {
                /* base values already set */
                continue;
            }
            /* compute using the linear recurrence */
            seq_t pos = 0;
            seq_t neg = 0;

            /* positive contributions: 2*a(i-1), 2*a(i-4), 2*a(i-6), 2*a(i-8) */
            pos += cache[i-1]; pos += cache[i-1];
            pos += cache[i-4]; pos += cache[i-4];
            pos += cache[i-6]; pos += cache[i-6];
            pos += cache[i-8]; pos += cache[i-8];

            /* negative contributions: a(i-2), 4*a(i-5), 2*a(i-9) */
            neg += cache[i-2];
            neg += cache[i-5]; neg += cache[i-5];
            neg += cache[i-5]; neg += cache[i-5];
            neg += cache[i-9]; neg += cache[i-9];

            cache[i] = pos - neg;   /* final result */
        }
        cache_last = n;
    }

    return cache[n];
}
