/*
 * A004858  Numbers that are the sum of at most 7 nonzero 6th powers.
 * https://oeis.org/A004858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve (DP over min number of nonzero 6th powers needed); list
 *            all m with min count <= 7
 * Range:     u8 n <= 26, u16 n <= 1016, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *a4858_cache = NULL;
static idx_t a4858_len = 0;

static void a4858_build(uint64_t limit) {
    uint8_t *mt = (uint8_t *)malloc((size_t)(limit + 1));
    memset(mt, 255, (size_t)(limit + 1));
    mt[0] = 0;
    for (uint64_t k = 1; ; k++) {
        uint64_t p = k * k * k * k * k * k;
        if (p > limit) break;
        for (uint64_t i = p; i <= limit; i++) {
            uint8_t prev = mt[i - p];
            if (prev < 7 && (uint8_t)(prev + 1) < mt[i]) mt[i] = (uint8_t)(prev + 1);
        }
    }
    idx_t count = 0;
    for (uint64_t i = 0; i <= limit; i++)
        if (mt[i] <= 7) count++;
    uint64_t *arr = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(count ? count : 1));
    idx_t c = 0;
    for (uint64_t i = 0; i <= limit; i++)
        if (mt[i] <= 7) arr[c++] = i;
    free(mt);
    if (a4858_cache) free(a4858_cache);
    a4858_cache = arr;
    a4858_len = count;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    uint64_t limit = (uint64_t)1 << 20;
    while (a4858_cache == NULL || n > a4858_len) {
        a4858_build(limit);
        limit *= 2;
    }
    return (seq_t)a4858_cache[n - 1];
}
