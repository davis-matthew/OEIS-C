/*
 * A004590  Expansion of sqrt(10) in base 7.
 * https://oeis.org/A004590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   digit-by-digit square root in base 7 of 10: start with p = 0, r
 *            = 0; at step k, r = 49*r + (k==1 ? 10 : 0), and a(k) = the
 *            largest d in 0..6 with (14*p + d)*d <= r; then r -= (14*p + d)*d
 *            and p = 7*p + d. So a(n) = floor(sqrt(10) * 7^(n-1)) mod 7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmpb, growl
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/cmpb.h"
#include "../utils/growl.h"

static uint32_t *P = 0, *R = 0, *Q = 0, *T = 0;
static size_t pl = 0, rl = 0, ql = 0, tl = 0, cap = 0;
static unsigned char *dig = 0;
static idx_t ndig = 0, dcap = 0;

static void trim(uint32_t *a, size_t *len) {
    while (*len > 0 && a[*len - 1] == 0) (*len)--;
}

static void mulsmall(uint32_t *a, size_t *len, uint32_t m, uint32_t c) {
    uint64_t carry = c;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t v = (uint64_t)a[i] * m + carry;
        a[i] = (uint32_t)v;
        carry = v >> 32;
    }
    if (carry) a[(*len)++] = (uint32_t)carry;
    trim(a, len);
}

static void subb(uint32_t *a, size_t *al, const uint32_t *b, size_t bl) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < *al; i++) {
        int64_t v = (int64_t)a[i] - borrow - (i < bl ? (int64_t)b[i] : 0);
        if (v < 0) { v += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)v;
    }
    trim(a, al);
}

static void extend(idx_t N) {
    size_t need = (size_t)(N / 10) + 16;
    if (need > cap) {
        P = growl(P, cap, need);
        R = growl(R, cap, need);
        Q = growl(Q, cap, need);
        T = growl(T, cap, need);
        cap = need;
    }
    if (N > dcap) {
        idx_t nc = N * 2 + 16;
        dig = (unsigned char *)realloc(dig, (size_t)nc);
        dcap = nc;
    }
    while (ndig < N) {
        int d;
        mulsmall(R, &rl, 49, ndig == 0 ? 10 : 0);
        memcpy(Q, P, pl * sizeof(uint32_t));
        ql = pl;
        mulsmall(Q, &ql, 14, 0);
        for (d = 6; d >= 0; d--) {
            memcpy(T, Q, ql * sizeof(uint32_t));
            tl = ql;
            mulsmall(T, &tl, 1, (uint32_t)d);
            mulsmall(T, &tl, (uint32_t)d, 0);
            if (cmpb(T, tl, R, rl) <= 0) break;
        }
        subb(R, &rl, T, tl);
        mulsmall(P, &pl, 7, (uint32_t)d);
        dig[ndig++] = (unsigned char)d;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > ndig) extend(n);
    return (seq_t)dig[n - 1];
}
