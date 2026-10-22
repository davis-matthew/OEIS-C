/*
 * A004814  Numbers that are the sum of 3 positive 11th powers.
 * https://oeis.org/A004814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: list every x^11 + y^11 + z^11 with 1 <= x <= y <= z <= m
 *            below (m+1)^11, sort, remove duplicates, and increase m until
 *            there are at least n terms
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 82, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

static u128 *vals = NULL;
static idx_t cnt = 0;
static idx_t curm = 0;

static int cmp_u128(const void *a, const void *b) {
    u128 x = *(const u128 *)a, y = *(const u128 *)b;
    return (x > y) - (x < y);
}

static u128 p11(idx_t b) {
    u128 p = 1;
    for (int i = 0; i < 11; i++) p *= (u128)b;
    return p;
}

static void build(idx_t m) {
    u128 *pows = (u128 *)malloc((size_t)(m + 2) * sizeof(u128));
    for (idx_t i = 1; i <= m + 1; i++) pows[i] = p11(i);
    u128 lim = pows[m + 1];
    idx_t total = (m + 2) * (m + 1) * m / 6 + 1;
    u128 *v = (u128 *)malloc((size_t)total * sizeof(u128));
    idx_t k = 0;
    for (idx_t x = 1; x <= m; x++) {
        for (idx_t y = x; y <= m; y++) {
            u128 s = pows[x] + pows[y];
            if (s >= lim) break;
            for (idx_t z = y; z <= m; z++) {
                u128 t = s + pows[z];
                if (t >= lim) break;
                v[k++] = t;
            }
        }
    }
    qsort(v, (size_t)k, sizeof(u128), cmp_u128);
    idx_t u = 0;
    for (idx_t i = 0; i < k; i++) {
        if (u == 0 || v[i] != v[u - 1]) v[u++] = v[i];
    }
    free(vals);
    vals = v;
    cnt = u;
    free(pows);
}

seq_t a(idx_t n) {
    while (cnt < n) {
        curm = curm < 8 ? 8 : curm + curm / 2;
        build(curm);
    }
    return u128_to_seq(vals[n - 1]);
}
