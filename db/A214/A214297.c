/*
 * A214297  a(0)=-1, a(1)=0, a(2)=-3; thereafter a(n+2) - 2*a(n+1) + a(n) has
 *          period 4: repeat -4, 8, -4, 2.
 * https://oeis.org/A214297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -1, a(1) = 0, a(2) = -3. Then for all n ≥ 0: a(n+2) =
 *            2*a(n+1) - a(n) + d(n), where the second difference d(n) is
 *            periodic with d(0)=−4, d(1)=8, d(2)=−4, d(3)=2, repeating every
 *            4
 * Range:     i8 n <= 22, i16 n <= 362, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;               /* cached sequence values */
    static idx_t  cache_len = 0;              /* number of values stored */
    static idx_t  cache_cap = 0;              /* allocated size of cache */

    /* Periodic second‑difference values: -4,8,-4,2 */
    static const int period[4] = { -4, 8, -4, 2 };

    /* Ensure cache has at least 3 entries, containing the initial values */
    if (cache_len == 0) {
        cache_cap = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)-3;
        cache_len = 3;
    }

    /* Grow cache if needed */
    if (n >= cache_len) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
        /* Compute new terms up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* d(i-2) where d repeats every 4 */
            int d = period[(i - 2) % 4];
            cache[i] = (seq_t)2 * cache[i - 1] - cache[i - 2] + (seq_t)d;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
