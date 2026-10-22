/*
 * A005350  a(1) = a(2) = a(3) = 1, a(n) = a(a(n-1)) + a(n-a(n-1)) for n >= 4.
 * https://oeis.org/A005350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(a(n-1)) + a(n-a(n-1)), a(1) = a(2) = a(3) = 1 (values
 *            satisfy 1 <= a(n) <= n, so they are stored as native indices)
 * Range:     u8 n <= 639, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t computed = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;

    if (n + 1 > cache_cap) {
        idx_t newcap = cache_cap ? cache_cap : 64;
        while (newcap < n + 1) newcap *= 2;
        cache = (uint64_t *)realloc(cache, newcap * sizeof(uint64_t));
        cache_cap = newcap;
    }

    if (computed == 0) {
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        computed = 3;
    }

    for (idx_t i = computed + 1; i <= n; i++) {
        uint64_t p = cache[i - 1];
        cache[i] = cache[p] + cache[i - p];
    }
    if (n > computed) computed = n;

    return (seq_t)cache[n];
}
