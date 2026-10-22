/*
 * A004810  Numbers that are the sum of 10 positive 10th powers.
 * https://oeis.org/A004810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate all multisets of 10 positive 10th powers
 *            with sum below (K+1)^10, sort and deduplicate, increasing K
 *            until at least n terms are found
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

static uint64_t *res_buf = NULL;
static size_t res_len = 0, res_cap = 0;

static uint64_t *cache = NULL;
static size_t cache_len = 0;

static uint64_t pw[100];

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u < v) ? -1 : (u > v) ? 1 : 0;
}

static void push(uint64_t v) {
    if (res_len == res_cap) {
        size_t nc = res_cap ? res_cap * 2 : 1024;
        uint64_t *nb = (uint64_t *)realloc(res_buf, nc * sizeof(uint64_t));
        if (!nb) return;
        res_buf = nb;
        res_cap = nc;
    }
    res_buf[res_len++] = v;
}

static void gen(uint64_t sum, int rem, int minb, int K, uint64_t L) {
    int b;
    if (rem == 0) {
        push(sum);
        return;
    }
    for (b = minb; b <= K; b++) {
        /* all remaining terms are at least pw[b] */
        if (pw[b] > (L - sum) / (uint64_t)rem) break;
        if (sum + pw[b] * (uint64_t)rem >= L) break;
        gen(sum + pw[b], rem - 1, b, K, L);
    }
}

static void build(int K) {
    int b, j;
    uint64_t L = 1;
    size_t i, m;
    for (b = 1; b <= K + 1; b++) {
        uint64_t p = 1;
        for (j = 0; j < 10; j++) p *= (uint64_t)b;
        pw[b] = p;
    }
    L = pw[K + 1];
    res_len = 0;
    gen(0, 10, 1, K, L);
    qsort(res_buf, res_len, sizeof(uint64_t), cmp_u64);
    m = 0;
    for (i = 0; i < res_len; i++) {
        if (m == 0 || res_buf[i] != res_buf[m - 1]) res_buf[m++] = res_buf[i];
    }
    free(cache);
    cache = (uint64_t *)malloc((m ? m : 1) * sizeof(uint64_t));
    if (!cache) { cache_len = 0; return; }
    for (i = 0; i < m; i++) cache[i] = res_buf[i];
    cache_len = m;
}

seq_t a(idx_t n) {
    static int curK = 1;
    if (n < 1) return (seq_t)0;
    while (cache_len < n && curK < 80) {
        curK++;
        build(curK);
    }
    if (cache_len < n) return (seq_t)0;
    return u128_to_seq((unsigned __int128)cache[n - 1]);
}
