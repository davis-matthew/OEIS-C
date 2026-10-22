/*
 * A004805  Numbers that are the sum of 5 positive 10th powers.
 * https://oeis.org/A004805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search - generate all sums p1^10+...+p5^10 with 1 <= p1 <= ...
 *            <= p5 <= B, keep those below (B+1)^10 + 4 (where the list is
 *            complete), sort, dedupe; grow B as needed
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 987, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

static u128 *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_B = 0;

static u128 p10(u128 x) {
    u128 x2 = x * x;
    u128 x4 = x2 * x2;
    u128 x5 = x4 * x;
    return x5 * x5;
}

static int cmp_u128(const void *a, const void *b) {
    u128 va = *(const u128 *)a;
    u128 vb = *(const u128 *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

static void build(idx_t B) {
    u128 limit = p10((u128)(B + 1)) + 4;
    u128 *pw = (u128 *)malloc((size_t)(B + 1) * sizeof(u128));
    idx_t cap = 1024, len = 0;
    u128 *buf = (u128 *)malloc((size_t)cap * sizeof(u128));
    idx_t i1, i2, i3, i4, i5;
    for (i1 = 0; i1 <= B; i1++) pw[i1] = p10((u128)i1);
    for (i1 = 1; i1 <= B; i1++) {
        u128 s1 = pw[i1];
        if (s1 * 5 >= limit) break;
        for (i2 = i1; i2 <= B; i2++) {
            u128 s2 = s1 + pw[i2];
            if (s1 + pw[i2] * 4 >= limit) break;
            for (i3 = i2; i3 <= B; i3++) {
                u128 s3 = s2 + pw[i3];
                if (s2 + pw[i3] * 3 >= limit) break;
                for (i4 = i3; i4 <= B; i4++) {
                    u128 s4 = s3 + pw[i4];
                    if (s3 + pw[i4] * 2 >= limit) break;
                    for (i5 = i4; i5 <= B; i5++) {
                        u128 s5 = s4 + pw[i5];
                        if (s5 >= limit) break;
                        if (len >= cap) {
                            cap *= 2;
                            buf = (u128 *)realloc(buf, (size_t)cap * sizeof(u128));
                        }
                        buf[len++] = s5;
                    }
                }
            }
        }
    }
    free(pw);
    qsort(buf, (size_t)len, sizeof(u128), cmp_u128);
    {
        idx_t w = 0, r;
        for (r = 0; r < len; r++)
            if (w == 0 || buf[r] != buf[w - 1]) buf[w++] = buf[r];
        len = w;
    }
    free(cache);
    cache = buf;
    cache_len = len;
    cache_B = B;
}

seq_t a(idx_t n) {
    if (cache_B == 0) build(8);
    while (cache_len < n) build(cache_B + cache_B / 2 + 1);
    return u128_to_seq(cache[n - 1]);
}
