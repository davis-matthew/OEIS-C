/*
 * A207721  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) +63*a(n-2) -85*a(n-3) -424*a(n-4) +387*a(n-5)
 *            +724*a(n-6) -695*a(n-7) -148*a(n-8) +185*a(n-9) +13*a(n-10)
 *            -14*a(n-11) -a(n-12) Initial terms: a(1)=22, a(2)=484,
 *            a(3)=5302, a(4)=66946, a(5)=820820, a(6)=10150118,
 *            a(7)=125165018, a(8)=1545006848, a(9)=19064074106,
 *            a(10)=235267721766...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* cached values for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    /* initial values (index 0 unused) */
    static const seq_t init[] = {
        0,  /* placeholder for index 0 */
        22, 484, 5302, 66946, 820820, 10150118,
        125165018, 1545006848, 19064074106, 235267721766,
        2903258039792, 35827609743354
    };
    const idx_t initial_len = sizeof(init) / sizeof(init[0]);  /* 13 */

    if (n < 1) return 0;  /* undefined for n<1, return 0 safely */

    /* initialise cache on first call */
    if (cache_size == 0) {
        cache_size = initial_len;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < initial_len; ++i) cache[i] = init[i];
    }

    /* grow cache if needed (double strategy) */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n) new_size *= 2;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* return known values */
    if (n < initial_len) return cache[n];

    /* compute missing values up to n */
    for (idx_t i = initial_len; i <= n; ++i) {
        seq_t pos = 8 * cache[i-1] + 63 * cache[i-2] + 387 * cache[i-5]
                  + 724 * cache[i-6] + 185 * cache[i-9] + 13 * cache[i-10];
        seq_t neg = 85 * cache[i-3] + 424 * cache[i-4] + 695 * cache[i-7]
                  + 148 * cache[i-8] + 14 * cache[i-11] + cache[i-12];
        cache[i] = pos - neg;   /* pos ≥ neg for all n≥13 */
    }

    return cache[n];
}
