/*
 * A004877  Numbers that are the sum of at most 4 nonzero 8th powers.
 * https://oeis.org/A004877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: enumerate all sums of at most 4 nonzero 8th powers below
 *            (B+1)^8, sort and dedupe, raising B until at least n terms are
 *            found
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 3676, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/u128_to_seq.h"

static int cmp_u64(const void *x, const void *y) {
    uint64_t va = *(const uint64_t *)x;
    uint64_t vb = *(const uint64_t *)y;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

static uint64_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_B = 0;

static void build(idx_t B) {
    uint64_t *pw = (uint64_t *)malloc((B + 2) * sizeof(uint64_t));
    idx_t i, j, k, l;
    for (i = 0; i <= B + 1; i++) {
        uint64_t p = 1;
        int e;
        for (e = 0; e < 8; e++) p *= (uint64_t)i;
        pw[i] = p;
    }
    uint64_t lim = pw[B + 1];
    size_t cap = 1024, cnt = 0;
    uint64_t *s = (uint64_t *)malloc(cap * sizeof(uint64_t));
#define PUSH(v) do { if (cnt == cap) { cap *= 2; s = (uint64_t *)realloc(s, cap * sizeof(uint64_t)); } s[cnt++] = (v); } while (0)
    PUSH(0);
    for (i = 1; i <= B; i++) {
        uint64_t s1 = pw[i];
        if (s1 >= lim) break;
        PUSH(s1);
        for (j = i; j <= B; j++) {
            uint64_t s2 = s1 + pw[j];
            if (s2 >= lim) break;
            PUSH(s2);
            for (k = j; k <= B; k++) {
                uint64_t s3 = s2 + pw[k];
                if (s3 >= lim) break;
                PUSH(s3);
                for (l = k; l <= B; l++) {
                    uint64_t s4 = s3 + pw[l];
                    if (s4 >= lim) break;
                    PUSH(s4);
                }
            }
        }
    }
#undef PUSH
    free(pw);
    qsort(s, cnt, sizeof(uint64_t), cmp_u64);
    size_t u = 0;
    for (i = 0; i < cnt; i++) {
        if (u == 0 || s[i] != s[u - 1]) s[u++] = s[i];
    }
    free(cache);
    cache = s;
    cache_len = (idx_t)u;
    cache_B = B;
}

seq_t a(idx_t n) {
    idx_t B = cache_B ? cache_B : 8;
    if (cache == NULL) build(B);
    while (cache_len < n && B < 200) {
        B *= 2;
        if (B > 200) B = 200;
        build(B);
    }
    if (n >= 1 && n <= cache_len) return u128_to_seq((unsigned __int128)cache[n - 1]);
    return (seq_t)0;
}
