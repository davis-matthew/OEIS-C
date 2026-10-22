/*
 * A004999  Sums of two nonnegative cubes.
 * https://oeis.org/A004999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   sieve -- list all x^3 + y^3 (0 <= x <= y) up to a bound, sort,
 *            remove duplicates; a(n) is the n-th smallest; double the bound
 *            when there are fewer than n values
 * Range:     u8 n <= 25, u16 n <= 747, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *cache = NULL;
static idx_t cache_len = 0;
static uint64_t cache_lim = 0;

static int cmp_u64(const void *p, const void *q) {
    uint64_t x = *(const uint64_t *)p, y = *(const uint64_t *)q;
    return (x > y) - (x < y);
}

static void build(uint64_t lim) {
    uint64_t cap = 1024, cnt = 0;
    uint64_t *v = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    uint64_t x, y;
    for (x = 0; x * x * x <= lim / 2 || x * x * x * 2 <= lim; x++) {
        uint64_t x3 = x * x * x;
        if (2 * x3 > lim) break;
        for (y = x;; y++) {
            uint64_t s = x3 + y * y * y;
            if (s > lim) break;
            if (cnt == cap) {
                cap *= 2;
                v = (uint64_t *)realloc(v, (size_t)cap * sizeof(uint64_t));
            }
            v[cnt++] = s;
        }
    }
    qsort(v, (size_t)cnt, sizeof(uint64_t), cmp_u64);
    {
        uint64_t i, u = 0;
        for (i = 0; i < cnt; i++)
            if (i == 0 || v[i] != v[i - 1]) v[u++] = v[i];
        cnt = u;
    }
    free(cache);
    cache = v;
    cache_len = (idx_t)cnt;
    cache_lim = lim;
}

seq_t a(idx_t n) {
    if (cache == NULL) build(1u << 16);
    while (cache_len < n) build(cache_lim * 2);
    return (seq_t)cache[n - 1];
}
