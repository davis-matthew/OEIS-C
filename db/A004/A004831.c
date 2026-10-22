/*
 * A004831  Numbers that are the sum of at most 2 nonzero 4th powers.
 * https://oeis.org/A004831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve/search: list all i^4 + j^4 with 0 <= i <= j up to a bound,
 *            sort, dedupe, take the n-th
 * Range:     u8 n <= 10, u16 n <= 130, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *cache = NULL;
static idx_t cache_len = 0;

static int cmp_u64(const void *p, const void *q) {
    uint64_t x = *(const uint64_t *)p, y = *(const uint64_t *)q;
    return (x > y) - (x < y);
}

static void build(uint64_t limit) {
    uint64_t K = 0;
    while ((K + 1) * (K + 1) * (K + 1) * (K + 1) <= limit) K++;
    size_t cap = (size_t)((K + 1) * (K + 2) / 2 + 1);
    uint64_t *v = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t cnt = 0;
    for (uint64_t i = 0; i <= K; i++) {
        uint64_t i4 = i * i * i * i;
        for (uint64_t j = i; j <= K; j++) {
            uint64_t s = i4 + j * j * j * j;
            if (s > limit) break;
            v[cnt++] = s;
        }
    }
    qsort(v, cnt, sizeof(uint64_t), cmp_u64);
    size_t w = 0;
    for (size_t r = 0; r < cnt; r++)
        if (w == 0 || v[w - 1] != v[r]) v[w++] = v[r];
    free(cache);
    cache = v;
    cache_len = (idx_t)w;
}

seq_t a(idx_t n) {
    if (n > cache_len) {
        uint64_t limit = 5 * (uint64_t)n * (uint64_t)n + 16;
        build(limit);
        while (cache_len < n) {
            limit *= 2;
            build(limit);
        }
    }
    return (seq_t)cache[n - 1];
}
