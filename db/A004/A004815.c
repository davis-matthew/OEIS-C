/*
 * A004815  Numbers that are the sum of 4 positive 11th powers.
 * https://oeis.org/A004815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: enumerate all sums a^11+b^11+c^11+d^11 with
 *            1<=a<=b<=c<=d<=m, keep those below (m+1)^11 (complete there),
 *            sort, deduplicate; grow m as needed
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 197, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

static u128 *terms = NULL;
static idx_t nterms = 0;
static int cur_m = 0;

static int cmp_u128(const void *x, const void *y) {
    u128 p = *(const u128 *)x, q = *(const u128 *)y;
    if (p < q) return -1;
    if (p > q) return 1;
    return 0;
}

static u128 p11(int x) {
    u128 r = 1;
    int i;
    for (i = 0; i < 11; i++) r *= (u128)x;
    return r;
}

static void build(int m) {
    u128 *pw = (u128 *)malloc((size_t)(m + 2) * sizeof(u128));
    u128 bound;
    size_t cap = 1024, cnt = 0, i, k;
    u128 *buf = (u128 *)malloc(cap * sizeof(u128));
    int a, b, c, d;
    for (a = 0; a <= m + 1; a++) pw[a] = p11(a);
    bound = pw[m + 1];
    for (a = 1; a <= m; a++) {
        if (4 * pw[a] >= bound) break;
        for (b = a; b <= m; b++) {
            u128 s2 = pw[a] + pw[b];
            if (s2 + 2 * pw[b] >= bound) break;
            for (c = b; c <= m; c++) {
                u128 s3 = s2 + pw[c];
                if (s3 + pw[c] >= bound) break;
                for (d = c; d <= m; d++) {
                    u128 s = s3 + pw[d];
                    if (s >= bound) break;
                    if (cnt == cap) {
                        cap *= 2;
                        buf = (u128 *)realloc(buf, cap * sizeof(u128));
                    }
                    buf[cnt++] = s;
                }
            }
        }
    }
    qsort(buf, cnt, sizeof(u128), cmp_u128);
    k = 0;
    for (i = 0; i < cnt; i++) {
        if (k == 0 || buf[i] != buf[k - 1]) buf[k++] = buf[i];
    }
    free(pw);
    free(terms);
    terms = buf;
    nterms = (idx_t)k;
    cur_m = m;
}

seq_t a(idx_t n) {
    while (terms == NULL || n > nterms) {
        build(cur_m == 0 ? 16 : cur_m + 8);
    }
    return u128_to_seq(terms[n - 1]);
}
