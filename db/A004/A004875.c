/*
 * A004875  Numbers that are the sum of at most 2 nonzero 8th powers.
 * https://oeis.org/A004875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: list all i^8 + j^8 with 0 <= i <= j <= K that are <=
 *            K^8, sort them, drop duplicates; double K until there are at
 *            least n terms
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 135, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128_t;

static u128_t *terms = NULL;
static idx_t terms_count = 0;

static int cmp_u128(const void *x, const void *y) {
    u128_t p = *(const u128_t *)x, q = *(const u128_t *)y;
    return (p > q) - (p < q);
}

static void build(idx_t need) {
    idx_t K = 16;
    for (;;) {
        u128_t *pw = (u128_t *)malloc((size_t)(K + 1) * sizeof(u128_t));
        idx_t i, j, cnt = 0, u = 0;
        u128_t lim, *buf;
        for (i = 0; i <= K; i++) {
            u128_t b = (u128_t)i, s = b * b;
            s = s * s;
            pw[i] = s * s;
        }
        lim = pw[K];
        buf = (u128_t *)malloc((size_t)((K + 1) * (K + 2) / 2) * sizeof(u128_t));
        for (i = 0; i <= K; i++) {
            if (pw[i] + pw[i] > lim) break;
            for (j = i; j <= K; j++) {
                u128_t s = pw[i] + pw[j];
                if (s > lim) break;
                buf[cnt++] = s;
            }
        }
        free(pw);
        qsort(buf, (size_t)cnt, sizeof(u128_t), cmp_u128);
        for (i = 0; i < cnt; i++) {
            if (u == 0 || buf[i] != buf[u - 1]) buf[u++] = buf[i];
        }
        if (u >= need) {
            if (terms) free(terms);
            terms = buf;
            terms_count = u;
            return;
        }
        free(buf);
        K *= 2;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (terms == NULL || n > terms_count) {
        idx_t need = n < 64 ? 64 : n;
        if (terms_count * 2 > need) need = terms_count * 2;
        build(need);
    }
    return u128_to_seq(terms[n - 1]);
}
