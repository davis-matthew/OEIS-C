/*
 * A001623  Number of 3 X n reduced (normalized) Latin rectangles.
 * https://oeis.org/A001623
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = N(n)/((n-1)*(n-2)), where N(n) = A000186(n) =
 *            Sum_{k=0..floor(n/2)} binomial(n,k)*D(k)*D(n-k)*U(n-2k)
 *            (Riordan). D = derangement numbers A000166: D(0) = 1, D(1) = 0,
 *            D(m) = m*D(m-1) + (-1)^m. U = menage numbers A000179: U(0) = 1,
 *            U(1) = -1, U(2) = 0, U(3) = 1, U(m) = ((m^2-2m)*U(m-1) + m*U...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   l1623_big_calc, l1623_is_fixed, l1623_ucst
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/l1623_big_calc.h"
#include "../utils/l1623_is_fixed.h"
#include "../utils/l1623_ucst.h"

#define L1623_LIMBS 16
#define L1623_MAXN 40

static void l1623_set(uint32_t *x, uint64_t v) {
    memset(x, 0, L1623_LIMBS * sizeof(uint32_t));
    x[0] = (uint32_t)v;
    x[1] = (uint32_t)(v >> 32);
}

static void l1623_add(uint32_t *x, const uint32_t *y) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < L1623_LIMBS; i++) {
        c += (uint64_t)x[i] + y[i];
        x[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void l1623_sub(uint32_t *x, const uint32_t *y) {
    int64_t b = 0;
    int i;
    for (i = 0; i < L1623_LIMBS; i++) {
        int64_t t = (int64_t)x[i] - (int64_t)y[i] - b;
        if (t < 0) { t += ((int64_t)1 << 32); b = 1; } else b = 0;
        x[i] = (uint32_t)t;
    }
}

static void l1623_mulsmall(uint32_t *x, uint32_t m) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < L1623_LIMBS; i++) {
        c += (uint64_t)x[i] * m;
        x[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void l1623_divsmall(uint32_t *x, uint32_t d) {
    uint64_t r = 0;
    int i;
    for (i = L1623_LIMBS - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static void l1623_mul(uint32_t *z, const uint32_t *x, const uint32_t *y) {
    uint32_t t[L1623_LIMBS];
    int i, j;
    memset(t, 0, sizeof(t));
    for (i = 0; i < L1623_LIMBS; i++) {
        uint64_t c = 0;
        if (x[i] == 0) continue;
        for (j = 0; i + j < L1623_LIMBS; j++) {
            c += (uint64_t)x[i] * y[j] + t[i + j];
            t[i + j] = (uint32_t)c;
            c >>= 32;
        }
    }
    memcpy(z, t, sizeof(t));
}

static seq_t l1623_fixed_calc(idx_t n) {
    static uint32_t D[L1623_MAXN + 1][L1623_LIMBS];
    static uint32_t U[L1623_MAXN + 1][L1623_LIMBS];
    uint32_t C[L1623_LIMBS], pos[L1623_LIMBS], neg[L1623_LIMBS];
    uint32_t p[L1623_LIMBS], tmp[L1623_LIMBS];
    idx_t k, m;
    int i;
    seq_t r = 0;

    l1623_set(D[0], 1);
    l1623_set(D[1], 0);
    for (m = 2; m <= n; m++) {
        memcpy(D[m], D[m - 1], sizeof(D[m]));
        l1623_add(D[m], D[m - 2]);
        l1623_mulsmall(D[m], (uint32_t)(m - 1));
    }
    l1623_set(U[0], 1);
    l1623_set(U[1], 1); /* magnitude; sign handled below */
    l1623_set(U[2], 0);
    l1623_set(U[3], 1);
    for (m = 4; m <= n; m++) {
        memcpy(U[m], U[m - 1], sizeof(U[m]));
        l1623_mulsmall(U[m], (uint32_t)(m * m - 2 * m));
        memcpy(tmp, U[m - 2], sizeof(tmp));
        l1623_mulsmall(tmp, (uint32_t)m);
        l1623_add(U[m], tmp);
        l1623_set(tmp, 4);
        if (m % 2 == 0) l1623_sub(U[m], tmp); else l1623_add(U[m], tmp);
        l1623_divsmall(U[m], (uint32_t)(m - 2));
    }

    l1623_set(C, 1);
    l1623_set(pos, 0);
    l1623_set(neg, 0);
    for (k = 0; 2 * k <= n; k++) {
        idx_t j = n - 2 * k;
        l1623_mul(p, C, D[k]);
        l1623_mul(p, p, D[n - k]);
        if (j == 1) {
            l1623_add(neg, p);
        } else {
            l1623_mul(p, p, U[j]);
            l1623_add(pos, p);
        }
        l1623_mulsmall(C, (uint32_t)(n - k));
        l1623_divsmall(C, (uint32_t)(k + 1));
    }
    l1623_sub(pos, neg);
    l1623_divsmall(pos, (uint32_t)(n - 1));
    l1623_divsmall(pos, (uint32_t)(n - 2));

    for (i = L1623_LIMBS - 1; i >= 0; i--) {
        r = (seq_t)(r << 16);
        r = (seq_t)(r << 16);
        r += (seq_t)pos[i];
    }
    return r;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;
    if (l1623_is_fixed()) {
        if (n > L1623_MAXN) return 0; /* value cannot fit any fixed width */
        return l1623_fixed_calc(n);
    }
    return l1623_big_calc(n);
}
