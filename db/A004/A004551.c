/*
 * A004551  Expansion of sqrt(3) in base 6.
 * https://oeis.org/A004551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(3) * 6^(n-1)) mod 6, computed by
 *            digit-by-digit base-6 square root with internal multiprecision
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_add, trim
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mul_add.h"
#include "../utils/trim.h"

static uint32_t *P = NULL, *R = NULL, *T = NULL;
static size_t plen = 0, rlen = 0, tlen = 0, cap = 0;
static unsigned char *dig = NULL;
static size_t ndig = 0, dcap = 0;

static int cmp_big(const uint32_t *x, size_t xl, const uint32_t *y, size_t yl) {
    size_t i;
    if (xl != yl) return xl < yl ? -1 : 1;
    for (i = xl; i > 0; i--) {
        if (x[i - 1] != y[i - 1]) return x[i - 1] < y[i - 1] ? -1 : 1;
    }
    return 0;
}

static void sub_big(uint32_t *x, size_t *xl, const uint32_t *y, size_t yl) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < *xl; i++) {
        int64_t v = (int64_t)x[i] - borrow - (i < yl ? (int64_t)y[i] : 0);
        if (v < 0) { v += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        x[i] = (uint32_t)v;
    }
    trim(x, xl);
}

static void ensure_cap(size_t k) {
    size_t need = (size_t)((k * 27) / 10 + 64) / 32 + 4;
    if (need > cap) {
        size_t nc = need * 2;
        uint32_t *np = (uint32_t *)calloc(nc, sizeof(uint32_t));
        uint32_t *nr = (uint32_t *)calloc(nc, sizeof(uint32_t));
        uint32_t *nt = (uint32_t *)calloc(nc, sizeof(uint32_t));
        if (P) { memcpy(np, P, plen * sizeof(uint32_t)); free(P); }
        if (R) { memcpy(nr, R, rlen * sizeof(uint32_t)); free(R); }
        if (T) free(T);
        P = np; R = nr; T = nt;
        cap = nc;
    }
}

static void step(void) {
    uint32_t d;
    ensure_cap(ndig + 2);
    mul_add(R, &rlen, 36, ndig == 0 ? 3u : 0u);
    for (d = 5; ; d--) {
        memcpy(T, P, plen * sizeof(uint32_t));
        tlen = plen;
        mul_add(T, &tlen, 12, d);
        mul_add(T, &tlen, d, 0);
        if (d == 0 || cmp_big(T, tlen, R, rlen) <= 0) break;
    }
    if (tlen > 0) sub_big(R, &rlen, T, tlen);
    mul_add(P, &plen, 6, d);
    if (ndig >= dcap) {
        size_t nc = dcap ? dcap * 2 : 64;
        unsigned char *nd = (unsigned char *)malloc(nc);
        if (dig) { memcpy(nd, dig, ndig); free(dig); }
        dig = nd;
        dcap = nc;
    }
    dig[ndig++] = (unsigned char)d;
}

seq_t a(idx_t n) {
    while (ndig < (size_t)n) step();
    return (seq_t)dig[n - 1];
}
