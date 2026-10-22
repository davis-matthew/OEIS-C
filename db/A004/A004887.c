/*
 * A004887  Numbers that are the sum of at most 3 positive 9th powers.
 * https://oeis.org/A004887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: sort and dedupe all sums x^9+y^9+z^9 with 0 <= x <= y <=
 *            z <= B; the sums below (B+1)^9 are complete, and B doubles until
 *            it covers n terms
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

static u128 *g_terms = NULL;
static idx_t g_count = 0;
static idx_t g_B = 0;

static u128 p9u(u128 b) {
    u128 r = 1;
    int i;
    for (i = 0; i < 9; i++) r *= b;
    return r;
}

static int cmp_u128(const void *x, const void *y) {
    u128 a1 = *(const u128 *)x, b1 = *(const u128 *)y;
    return (a1 > b1) - (a1 < b1);
}

static void build(idx_t B) {
    idx_t i, j, k, cnt = 0, cap;
    u128 limit = p9u((u128)(B + 1));
    u128 *pw = (u128 *)malloc((size_t)(B + 1) * sizeof(u128));
    u128 *buf;
    for (i = 0; i <= B; i++) pw[i] = (i == 0) ? 0 : p9u((u128)i);
    cap = (B + 1) * (B + 2) * (B + 3) / 6 + 1;
    buf = (u128 *)malloc((size_t)cap * sizeof(u128));
    for (i = 0; i <= B; i++) {
        if (pw[i] >= limit) break;
        for (j = i; j <= B; j++) {
            u128 s2 = pw[i] + pw[j];
            if (s2 >= limit) break;
            for (k = j; k <= B; k++) {
                u128 s3 = s2 + pw[k];
                if (s3 >= limit) break;
                buf[cnt++] = s3;
            }
        }
    }
    free(pw);
    qsort(buf, (size_t)cnt, sizeof(u128), cmp_u128);
    {
        idx_t u = 0;
        for (i = 0; i < cnt; i++)
            if (u == 0 || buf[i] != buf[u - 1]) buf[u++] = buf[i];
        cnt = u;
    }
    if (g_terms) free(g_terms);
    g_terms = buf;
    g_count = cnt;
    g_B = B;
}

seq_t a(idx_t n) {
    idx_t B = g_B ? g_B : 8;
    if (n == 0) return (seq_t)0;
    if (!g_terms) build(B);
    while (g_count < n) {
        B *= 2;
        build(B);
    }
    return u128_to_seq(g_terms[n - 1]);
}
