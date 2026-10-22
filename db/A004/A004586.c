/*
 * A004586  Expansion of sqrt(10) in base 3.
 * https://oeis.org/A004586
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(10)*3^(n-3)) mod 3, the base-3 digits of
 *            sqrt(10) found one at a time by digit-by-digit integer square
 *            root: p = floor(sqrt(10*9^k)), r = 10*9^k - p^2, p(0)=3, r(0)=1;
 *            next digit d = max d in {0,1,2} with 9r - (6p*d + d^2) >= 0,
 *            then p -> 3p+d, r -> 9r - 6pd - d^2. a(2)=1, a(3)=...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *P = NULL, *R = NULL, *T = NULL, *U = NULL;
static size_t cap = 0, plen = 0, rlen = 0;
static unsigned char *dig = NULL;
static size_t ndig = 0, digcap = 0;

static void ensure_cap(size_t need) {
    size_t nc;
    if (need <= cap) return;
    nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    P = (uint32_t *)realloc(P, nc * sizeof(uint32_t));
    R = (uint32_t *)realloc(R, nc * sizeof(uint32_t));
    T = (uint32_t *)realloc(T, nc * sizeof(uint32_t));
    U = (uint32_t *)realloc(U, nc * sizeof(uint32_t));
    cap = nc;
}

/* dst = src*m + add; dst may equal src */
static size_t muladd(uint32_t *dst, const uint32_t *src, size_t len, uint32_t m, uint32_t add) {
    uint64_t c = add;
    size_t i;
    for (i = 0; i < len; i++) {
        c += (uint64_t)src[i] * m;
        dst[i] = (uint32_t)c;
        c >>= 32;
    }
    while (c) {
        dst[len++] = (uint32_t)c;
        c >>= 32;
    }
    while (len > 0 && dst[len - 1] == 0) len--;
    return len;
}

static int cmp_big(const uint32_t *x, size_t lx, const uint32_t *y, size_t ly) {
    size_t i;
    if (lx != ly) return lx < ly ? -1 : 1;
    for (i = lx; i > 0; i--) {
        if (x[i - 1] != y[i - 1]) return x[i - 1] < y[i - 1] ? -1 : 1;
    }
    return 0;
}

/* x -= y, requires x >= y */
static size_t sub_big(uint32_t *x, size_t lx, const uint32_t *y, size_t ly) {
    int64_t b = 0;
    size_t i;
    for (i = 0; i < lx; i++) {
        int64_t v = (int64_t)x[i] - b - (i < ly ? (int64_t)y[i] : 0);
        if (v < 0) { v += ((int64_t)1 << 32); b = 1; } else b = 0;
        x[i] = (uint32_t)v;
    }
    while (lx > 0 && x[lx - 1] == 0) lx--;
    return lx;
}

static void push_digit(unsigned char d) {
    if (ndig >= digcap) {
        size_t nc = digcap ? digcap * 2 : 64;
        dig = (unsigned char *)realloc(dig, nc);
        digcap = nc;
    }
    dig[ndig++] = d;
}

seq_t a(idx_t n) {
    size_t want;
    if (n < 2) return 0;
    if (ndig == 0) {
        ensure_cap(16);
        P[0] = 3; plen = 1;
        R[0] = 1; rlen = 1;
        push_digit(1);
        push_digit(0);
    }
    want = (size_t)(n - 2);
    while (ndig <= want) {
        size_t tlen, ulen, mx;
        unsigned char d = 0;
        mx = plen > rlen ? plen : rlen;
        ensure_cap(mx + 4);
        rlen = muladd(R, R, rlen, 9, 0);
        tlen = muladd(T, P, plen, 6, 1);          /* 6p + 1 */
        if (cmp_big(R, rlen, T, tlen) >= 0) {
            ulen = muladd(U, T, tlen, 2, 2);      /* 12p + 4 */
            if (cmp_big(R, rlen, U, ulen) >= 0) {
                d = 2;
                rlen = sub_big(R, rlen, U, ulen);
            } else {
                d = 1;
                rlen = sub_big(R, rlen, T, tlen);
            }
        }
        plen = muladd(P, P, plen, 3, d);
        push_digit(d);
    }
    return (seq_t)dig[want];
}
