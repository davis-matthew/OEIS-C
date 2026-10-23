/*
 * A120424  Having specified two initial terms, the "Half-Fibonacci" sequence
 *          proceeds like the Fibonacci sequence, except that the terms are
 *          halved before being added if they are even.
 * https://oeis.org/A120424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (a(n-1) if a(n-1) is odd else a(n-1)/2) + (a(n-2) if
 *            a(n-2) is odd else a(n-2)/2), with a(0)=1, a(1)=3
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 87, u64 n <= 177,
 *            u128 n <= 370, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Ensure sufficient capacity for index n */
    if (n + 1 > cache_cap) {
        idx_t newcap = (n + 1) < 8 ? 8 : (n + 1) * 2;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }

    /* Initialise first two terms when required */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 1) {
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        if (i == 1) {
            cache[1] = (seq_t)3;
            continue;
        }
        seq_t v1 = cache[i - 1];
        seq_t v2 = cache[i - 2];
        seq_t term1 = (v1 & (seq_t)1) ? v1 : (v1 >> 1);
        seq_t term2 = (v2 & (seq_t)1) ? v2 : (v2 >> 1);
        cache[i] = term1 + term2;
    }
    cache_len = n + 1;
    return cache[n];
}
