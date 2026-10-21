/*
 * A053441  Moments of generalized Motzkin paths.
 * https://oeis.org/A053441
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4*a(n-2) + 2*a(n-3) - a(n-6); a(2)=1, a(3)=0, a(4)=4,
 *            a(5)=4, a(6)=16, a(7)=24
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 58, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* undefined before the offset; return 0 */

    /* static cache for computed terms */
    static seq_t cache[12000];
    static size_t built = 0;   /* number of terms computed (index+1) */

    /* initialize base terms once */
    if (built == 0) {
        cache[0] = 1;   /* a(2) */
        cache[1] = 0;   /* a(3) */
        cache[2] = 4;   /* a(4) */
        cache[3] = 4;   /* a(5) */
        cache[4] = 16;  /* a(6) */
        cache[5] = 24;  /* a(7) */
        built = 6;      /* indices 0..5 are ready */
    }

    idx_t idx = n - 2;   /* zero‑based index in the cache */

    if ((size_t)idx >= built) {
        /* compute missing terms up to idx */
        for (idx_t i = built; i <= idx; ++i) {
            /* recurrence valid for n >= 8 which corresponds to i >= 6 */
            cache[i] = 4 * cache[i - 2] + 2 * cache[i - 3] - cache[i - 6];
        }
        built = idx + 1;
    }

    return cache[(size_t)idx];
}
