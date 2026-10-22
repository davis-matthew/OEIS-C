/*
 * A004546  Expansion of sqrt(2) in base 9.
 * https://oeis.org/A004546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   digit-by-digit square root of 2 in base 9: r(1) = 2, p = 0; at
 *            each step r <- 81*r (after the first step), a(n) = largest d in
 *            0..8 with (18*p + d)*d <= r, then r <- r - (18*p + d)*d and p <-
 *            9*p + d. Big numbers kept as uint32 limb arrays
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/cmp_big.h"

static uint32_t *R = 0, *P = 0, *T = 0;
static size_t rl = 0, pl = 0, tl = 0, cap = 0;
static unsigned char *D = 0;
static idx_t dn = 0, dcap = 0;

static void ensure_cap(size_t need) {
    if (need <= cap) return;
    size_t nc = need * 2 + 16;
    R = (uint32_t *)realloc(R, nc * sizeof(uint32_t));
    P = (uint32_t *)realloc(P, nc * sizeof(uint32_t));
    T = (uint32_t *)realloc(T, nc * sizeof(uint32_t));
    cap = nc;
}

static void mul_small(uint32_t *x, size_t *l, uint32_t m, uint32_t add) {
    uint64_t c = add;
    size_t i;
    for (i = 0; i < *l; i++) {
        c += (uint64_t)x[i] * m;
        x[i] = (uint32_t)c;
        c >>= 32;
    }
    while (c) {
        x[(*l)++] = (uint32_t)c;
        c >>= 32;
    }
}

static void step(idx_t i) {
    size_t need = (rl > pl ? rl : pl) + 4;
    ensure_cap(need);
    if (i > 1) mul_small(R, &rl, 81, 0);
    uint32_t d;
    for (d = 8;; d--) {
        uint64_t c = (uint64_t)d * d;
        uint32_t m = 18 * d;
        size_t k;
        for (k = 0; k < pl; k++) {
            c += (uint64_t)P[k] * m;
            T[k] = (uint32_t)c;
            c >>= 32;
        }
        tl = pl;
        while (c) {
            T[tl++] = (uint32_t)c;
            c >>= 32;
        }
        while (tl > 0 && T[tl - 1] == 0) tl--;
        if (cmp_big(T, tl, R, rl) <= 0) break;
        if (d == 0) break;
    }
    /* R -= T */
    {
        uint64_t borrow = 0;
        size_t k;
        for (k = 0; k < rl; k++) {
            uint64_t sub = (k < tl ? (uint64_t)T[k] : 0) + borrow;
            uint64_t cur = (uint64_t)R[k];
            if (cur >= sub) {
                R[k] = (uint32_t)(cur - sub);
                borrow = 0;
            } else {
                R[k] = (uint32_t)(cur + ((uint64_t)1 << 32) - sub);
                borrow = 1;
            }
        }
        while (rl > 0 && R[rl - 1] == 0) rl--;
    }
    mul_small(P, &pl, 9, d);
    if (i >= dcap) {
        idx_t nc = i * 2 + 64;
        D = (unsigned char *)realloc(D, (size_t)nc);
        dcap = nc;
    }
    D[i] = (unsigned char)d;
}

seq_t a(idx_t n) {
    if (dn == 0) {
        ensure_cap(16);
        R[0] = 2;
        rl = 1;
        pl = 0;
    }
    while (dn < n) {
        dn++;
        step(dn);
    }
    return (seq_t)D[n];
}
