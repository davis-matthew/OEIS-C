/*
 * A003076  n-th digit after decimal point of square root of n.
 * https://oeis.org/A003076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A000196(n*100^n) mod 10, computed by the decimal
 *            digit-by-digit (long-hand) square root algorithm on base-10^9
 *            limbs
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3076_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a3076_cmp.h"

#define A3076_BASE 1000000000u

static size_t a3076_mad(uint32_t *x, size_t len, uint32_t m, uint32_t c) {
    uint64_t carry = c;
    size_t i;
    for (i = 0; i < len; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(v % A3076_BASE);
        carry = v / A3076_BASE;
    }
    while (carry) {
        x[len++] = (uint32_t)(carry % A3076_BASE);
        carry /= A3076_BASE;
    }
    while (len && x[len - 1] == 0) len--;
    return len;
}

static size_t a3076_sub(uint32_t *x, size_t lx, const uint32_t *y, size_t ly) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < lx; i++) {
        int64_t v = (int64_t)x[i] - borrow - (i < ly ? (int64_t)y[i] : 0);
        if (v < 0) { v += A3076_BASE; borrow = 1; } else borrow = 0;
        x[i] = (uint32_t)v;
    }
    while (lx && x[lx - 1] == 0) lx--;
    return lx;
}

seq_t a(idx_t n) {
    int dg[24];
    int nd = 0;
    idx_t t = n;
    idx_t g, total, s;
    size_t cap, lr = 0, lp = 0, ly, lt;
    uint32_t *R, *P, *Y, *T;
    unsigned res;

    do { dg[nd++] = (int)(t % 10); t /= 10; } while (t);
    g = (idx_t)((nd + 1) / 2);
    total = g + n;

    cap = (size_t)((n + 60) / 9 + 8);
    R = (uint32_t *)calloc(cap, sizeof(uint32_t));
    P = (uint32_t *)calloc(cap, sizeof(uint32_t));
    Y = (uint32_t *)calloc(cap, sizeof(uint32_t));
    T = (uint32_t *)calloc(cap, sizeof(uint32_t));

    for (s = 0; s < total; s++) {
        uint32_t pr = 0;
        int start, x;
        if (s < g) {
            int j = (int)(g - 1 - s);
            pr = (uint32_t)dg[2 * j];
            if (2 * j + 1 < nd) pr += 10u * (uint32_t)dg[2 * j + 1];
        }
        lr = a3076_mad(R, lr, 100, pr);

        memcpy(Y, P, lp * sizeof(uint32_t));
        ly = a3076_mad(Y, lp, 20, 0);

        if (lr == 0 || ly > lr) {
            start = 0;
        } else {
            double rd = 0.0, yd = 0.0;
            int i;
            for (i = 0; i < 3 && (size_t)i < lr; i++) {
                size_t idx = lr - 1 - (size_t)i;
                rd = rd * 1e9 + (double)R[idx];
                yd = yd * 1e9 + (idx < ly ? (double)Y[idx] : 0.0);
            }
            if (yd == 0.0) start = 9;
            else {
                double q = rd / yd;
                start = q >= 9.0 ? 9 : (int)q + 1;
                if (start > 9) start = 9;
            }
        }

        lt = 0;
        for (x = start; x >= 0; x--) {
            memcpy(T, Y, ly * sizeof(uint32_t));
            lt = a3076_mad(T, ly, 1, (uint32_t)x);
            lt = a3076_mad(T, lt, (uint32_t)x, 0);
            if (a3076_cmp(T, lt, R, lr) <= 0) break;
        }
        if (x < 0) { x = 0; lt = 0; }

        lr = a3076_sub(R, lr, T, lt);
        lp = a3076_mad(P, lp, 10, (uint32_t)x);
    }

    res = lp ? P[0] % 10 : 0;
    free(R); free(P); free(Y); free(T);
    return (seq_t)res;
}
