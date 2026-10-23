/*
 * A186561  Number of (n+2) X 4 0..2 arrays with every 3 X 3 subblock
 *          commuting with each horizontal and vertical neighbor 3 X 3
 *          subblock.
 * https://oeis.org/A186561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 4*a(n-4) + 2*a(n-5) +
 *            2*a(n-6) - 2*a(n-7), for n > 10. Initial values: a(1)=721,
 *            a(2)=173, a(3)=401, a(4)=1411, a(5)=2617, a(6)=3379, a(7)=4033,
 *            a(8)=4795, a(9)=6361, a(10)=10443
 * Range:     u8 none, u16 n <= 13, u32 n <= 40, u64 n <= 91, u128 n <= 195,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for i>=1; index 0 unused */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size (>= n+1) */
    static idx_t max_computed = 0;/* highest index computed so far */

    /* base values (index 0 unused) */
    static const seq_t base_vals[11] = {
        0,      /* unused 0 */
        721,    /* a(1) */
        173,    /* a(2) */
        401,    /* a(3) */
        1411,   /* a(4) */
        2617,   /* a(5) */
        3379,   /* a(6) */
        4033,   /* a(7) */
        4795,   /* a(8) */
        6361,   /* a(9) */
        10443   /* a(10) */
    };

    if (n < 1)          /* sequence is defined for n>=1 only */
        return 0;

    /* Resize cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failure – return 0 as a safe fallback */
            return 0;
        }
        /* Copy existing values if any */
        if (cache) {
            for (idx_t i = 1; i <= max_computed; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = base_vals[i];
        } else {
            seq_t pos = 2 * cache[i - 1] + 2 * cache[i - 3]
                      + 2 * cache[i - 5] + 2 * cache[i - 6];
            seq_t neg = cache[i - 2] + 4 * cache[i - 4] + 2 * cache[i - 7];
            cache[i] = pos - neg;
        }
    }
    max_computed = n;
    return cache[n];
}
