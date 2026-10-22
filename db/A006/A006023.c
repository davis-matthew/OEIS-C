/*
 * A006023  Number of unlabeled mating digraphs with n nodes.
 * https://oeis.org/A006023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] Sum_{m>=1} x^m * Sum_{(j) |- m} h((j)) * 2^Y((j)) *
 *            Prod_i (1-x^i)^{j_i}, a(0) = 1, with h((j)) = 1/Prod_i (j_i! *
 *            i^{j_i}) and Y((j)) = Sum_i (i-1)*j_i + Sum_i i*j_i*(j_i-1) +
 *            2*Sum_{r<s} gcd(r,s)*j_r*j_s (the loopless-digraph exponent).
 *            Expanding (1-x^i)^{j_i} splits (j) into s |-...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   div_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/div_small.h"

static uint32_t *Pacc, *Qacc, *Wt, *Fac;
static size_t Lacc, FL;
static long Nn, Bcur;
static long *sm, *tm;
static uint64_t dbuf;

static void mul_small(uint32_t *x, size_t len, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        c += (uint64_t)x[i] * m;
        x[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void add_shift(uint32_t *acc, size_t alen, const uint32_t *x, size_t xlen, uint64_t sh) {
    size_t q = (size_t)(sh / 32);
    unsigned b = (unsigned)(sh % 32);
    uint64_t carry = 0;
    size_t i, k;
    for (i = 0; i <= xlen; i++) {
        uint64_t hi = (i < xlen) ? ((uint64_t)x[i] << b) : 0;
        uint64_t lo = (i > 0) ? (((uint64_t)x[i - 1]) >> (32 - b)) : 0;
        uint32_t piece = (uint32_t)(hi | lo);
        if (q + i >= alen) return;
        carry += (uint64_t)acc[q + i] + piece;
        acc[q + i] = (uint32_t)carry;
        carry >>= 32;
    }
    for (k = q + xlen + 1; carry && k < alen; k++) {
        carry += acc[k];
        acc[k] = (uint32_t)carry;
        carry >>= 32;
    }
}

static void wpush(uint64_t f) {
    if (f <= 1) return;
    if (dbuf * f > 0xFFFFFFFFULL) {
        div_small(Wt, FL, (uint32_t)dbuf);
        dbuf = f;
    } else {
        dbuf *= f;
    }
}

static uint64_t gcd_u(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static void process(void) {
    long i, k, cntd = 0, tparts = 0, ia, ib;
    uint64_t Y = 0, sumc = 0;
    long *parts = (long *)malloc((size_t)(Nn + 1) * sizeof(long));
    long *cm = (long *)malloc((size_t)(Nn + 1) * sizeof(long));
    memcpy(Wt, Fac, FL * sizeof(uint32_t));
    dbuf = 1;
    for (i = 1; i <= Nn; i++) {
        for (k = 1; k <= sm[i]; k++) { wpush((uint64_t)k); wpush((uint64_t)i); }
        for (k = 1; k <= tm[i]; k++) { wpush((uint64_t)k); wpush((uint64_t)i); }
        tparts += tm[i];
        if (sm[i] + tm[i] > 0) {
            parts[cntd] = i;
            cm[cntd] = sm[i] + tm[i];
            cntd++;
        }
    }
    if (dbuf > 1) div_small(Wt, FL, (uint32_t)dbuf);
    for (ia = 0; ia < cntd; ia++) {
        sumc += (uint64_t)cm[ia];
        for (ib = 0; ib < cntd; ib++)
            Y += gcd_u((uint64_t)parts[ia], (uint64_t)parts[ib]) * (uint64_t)cm[ia] * (uint64_t)cm[ib];
    }
    Y -= sumc;
    if (tparts & 1) add_shift(Qacc, Lacc, Wt, FL, Y);
    else add_shift(Pacc, Lacc, Wt, FL, Y);
    free(parts);
    free(cm);
}

static void gen(int stage, long rem, long maxp) {
    long *mult = stage ? tm : sm;
    long p, k;
    if (rem == 0) {
        if (stage == 0) gen(1, Bcur, Bcur);
        else process();
        return;
    }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        for (k = 1; k * p <= rem; k++) {
            mult[p] = k;
            gen(stage, rem - k * p, p - 1);
        }
        mult[p] = 0;
    }
}

seq_t a(idx_t n) {
    long m, i, top;
    size_t bits;
    seq_t result = 0;
    if (n == 0) return 1;
    Nn = (long)n;
    bits = (size_t)n * (size_t)n + 32 * (size_t)n + 128;
    Lacc = bits / 32 + 2;
    Pacc = (uint32_t *)calloc(Lacc, sizeof(uint32_t));
    Qacc = (uint32_t *)calloc(Lacc, sizeof(uint32_t));
    FL = (size_t)n + 2;
    Fac = (uint32_t *)calloc(FL, sizeof(uint32_t));
    Wt = (uint32_t *)calloc(FL, sizeof(uint32_t));
    Fac[0] = 1;
    for (i = 2; i <= Nn; i++) mul_small(Fac, FL, (uint32_t)i);
    while (FL > 1 && Fac[FL - 1] == 0) FL--;
    sm = (long *)calloc((size_t)(Nn + 1), sizeof(long));
    tm = (long *)calloc((size_t)(Nn + 1), sizeof(long));

    for (m = (Nn + 1) / 2; m <= Nn; m++) {
        long A = 2 * m - Nn;
        Bcur = Nn - m;
        gen(0, A, A);
    }

    /* R = P - Q */
    {
        int64_t borrow = 0;
        size_t j;
        for (j = 0; j < Lacc; j++) {
            int64_t v = (int64_t)Pacc[j] - (int64_t)Qacc[j] - borrow;
            if (v < 0) { v += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
            Pacc[j] = (uint32_t)v;
        }
    }
    for (i = 2; i <= Nn; i++) div_small(Pacc, Lacc, (uint32_t)i);

    top = (long)Lacc - 1;
    while (top > 0 && Pacc[top] == 0) top--;
    for (i = top; i >= 0; i--) {
        int nib;
        for (nib = 7; nib >= 0; nib--) {
            result = result * 16 + (seq_t)((Pacc[i] >> (4 * nib)) & 15u);
        }
    }

    free(Pacc); free(Qacc); free(Fac); free(Wt); free(sm); free(tm);
    return result;
}
