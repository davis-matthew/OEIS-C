/*
 * A004900  Numbers that are the sum of at most 5 nonzero 10th powers.
 * https://oeis.org/A004900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve/search — enumerate all sums of at most 5 nonzero 10th
 *            powers with bases <= B, keep those below (B+1)^10 (which are
 *            then complete), sort and deduplicate; a(n) is the n-th smallest
 *            (a(1) = 0)
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   pow10u, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/pow10u.h"

static uint64_t *cache = NULL;
static idx_t cache_cnt = 0;
static int cache_B = 0;

static int cmp_u64(const void *x, const void *y) {
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return (p > q) - (p < q);
}

static void build(int B) {
    uint64_t pw[80];
    for (int i = 0; i <= B; i++) pw[i] = pow10u((uint64_t)i);
    uint64_t limit = pow10u((uint64_t)B + 1);
    size_t cap = 1024, cnt = 0;
    uint64_t *buf = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (int b1 = 0; b1 <= B; b1++) {
        uint64_t s1 = pw[b1];
        if (s1 >= limit) break;
        for (int b2 = b1; b2 <= B; b2++) {
            uint64_t s2 = s1 + pw[b2];
            if (s2 >= limit) break;
            for (int b3 = b2; b3 <= B; b3++) {
                uint64_t s3 = s2 + pw[b3];
                if (s3 >= limit) break;
                for (int b4 = b3; b4 <= B; b4++) {
                    uint64_t s4 = s3 + pw[b4];
                    if (s4 >= limit) break;
                    for (int b5 = b4; b5 <= B; b5++) {
                        uint64_t s5 = s4 + pw[b5];
                        if (s5 >= limit) break;
                        if (cnt == cap) {
                            cap *= 2;
                            buf = (uint64_t *)realloc(buf, cap * sizeof(uint64_t));
                        }
                        buf[cnt++] = s5;
                    }
                }
            }
        }
    }
    qsort(buf, cnt, sizeof(uint64_t), cmp_u64);
    size_t u = 0;
    for (size_t i = 0; i < cnt; i++)
        if (u == 0 || buf[i] != buf[u - 1]) buf[u++] = buf[i];
    free(cache);
    cache = buf;
    cache_cnt = (idx_t)u;
    cache_B = B;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    int B = cache_B;
    while (cache_cnt < n) {
        if (B >= 70) break;
        B = (B < 4) ? 4 : B + B / 2;
        if (B > 70) B = 70;
        build(B);
    }
    if (n > cache_cnt) return 0;
    return u128_to_seq((unsigned __int128)cache[n - 1]);
}
