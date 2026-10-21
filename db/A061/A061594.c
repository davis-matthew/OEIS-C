/*
 * A061594  Number of ways to place 3n nonattacking kings on a 6 X 2n
 *          chessboard.
 * https://oeis.org/A061594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 19*a(n-1) -
 *            148*a(n-2) + 604*a(n-3) * - 1364*a(n-4) + 1644*a(n-5) -
 *            928*a(n-6) + 192*a(n-7) * with initial values a(0)=1, a(1)=32,
 *            a(2)=408, a(3)=3600, * a(4)=26040, a(5)=166368, a(6)=976640
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 26, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static const seq_t init[7] = {1, 32, 408, 3600, 26040, 166368, 976640};

    /* Allocate or grow cache to hold indices up to n */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure → return 0 */
        cache = tmp;
        /* If cache was newly allocated, initialise lower entries */
        if (cache_len == 0) {
            for (idx_t i = 0; i < 7 && i < n + 1; ++i)
                cache[i] = init[i];
        }
        /* Compute new terms recursively */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 7) {                 /* initial values already set */
                cache[i] = init[i];
                continue;
            }
#if defined(__INT128__) || defined(__SIZEOF_INT128__)
            /* 64‑bit build: use 128‑bit intermediate to avoid overflow */
            if (sizeof(seq_t) <= sizeof(unsigned long long)) {
                __int128 t =
                    (__int128)cache[i-1] * 19
                    - (__int128)cache[i-2] * 148
                    + (__int128)cache[i-3] * 604
                    - (__int128)cache[i-4] * 1364
                    + (__int128)cache[i-5] * 1644
                    - (__int128)cache[i-6] * 928
                    + (__int128)cache[i-7] * 192;
                cache[i] = (seq_t)t;
            } else
#endif
            {
                /* Arbitrary‑precision build: rely on seq_t arithmetic */
                cache[i] = 19  * cache[i-1]
                         - 148 * cache[i-2]
                         + 604 * cache[i-3]
                         - 1364 * cache[i-4]
                         + 1644 * cache[i-5]
                         - 928  * cache[i-6]
                         + 192  * cache[i-7];
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
