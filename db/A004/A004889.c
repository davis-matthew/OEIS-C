/*
 * A004889  Numbers that are the sum of at most 5 positive 9th powers.
 * https://oeis.org/A004889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all sums of at most 5 ninth powers k^9 (k >=
 *            0) below (B+1)^9, sort, dedupe; grow B until n terms exist
 * Range:     u8 n <= 6, u16 n <= 52, u32 n <= 3935, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static uint64_t *cache = NULL;
static idx_t cache_len = 0;

static void build(idx_t need) {
    idx_t B = 8;
    for (;;) {
        uint64_t p[64];
        idx_t k;
        int i;
        for (k = 0; k <= B + 1; k++) {
            uint64_t v = 1;
            for (i = 0; i < 9; i++) v *= (uint64_t)k;
            p[k] = v;
        }
        uint64_t limit = p[B + 1];
        /* number of multisets of size 5 from B+1 values: C(B+5,5) */
        uint64_t cap = 1;
        for (k = 1; k <= 5; k++) cap = cap * (uint64_t)(B + k) / (uint64_t)k;
        uint64_t *s = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
        idx_t cnt = 0;
        idx_t a1, a2, a3, a4, a5;
        for (a1 = 0; a1 <= B; a1++) {
            uint64_t s1 = p[a1];
            if (s1 >= limit) break;
            for (a2 = a1; a2 <= B; a2++) {
                uint64_t s2 = s1 + p[a2];
                if (s2 >= limit) break;
                for (a3 = a2; a3 <= B; a3++) {
                    uint64_t s3 = s2 + p[a3];
                    if (s3 >= limit) break;
                    for (a4 = a3; a4 <= B; a4++) {
                        uint64_t s4 = s3 + p[a4];
                        if (s4 >= limit) break;
                        for (a5 = a4; a5 <= B; a5++) {
                            uint64_t s5 = s4 + p[a5];
                            if (s5 >= limit) break;
                            s[cnt++] = s5;
                        }
                    }
                }
            }
        }
        qsort(s, (size_t)cnt, sizeof(uint64_t), cmp_u64);
        idx_t u = 0;
        for (k = 0; k < cnt; k++)
            if (u == 0 || s[k] != s[u - 1]) s[u++] = s[k];
        if (u >= need || B >= 60) {
            if (cache) free(cache);
            cache = s;
            cache_len = u;
            return;
        }
        free(s);
        B += 4;
    }
}

seq_t a(idx_t n) {
    if (cache == NULL || n > cache_len) build(n);
    return u128_to_seq((unsigned __int128)cache[n - 1]);
}
