/*
 * A002272  Theta series of 32-dimensional Quebbemann lattice Q_32.
 * https://oeis.org/A002272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. b(x)^8 - 192*b(x)^4*d(x) + 576*d(x)^2, where b(x)^4 =
 *            (E8(x) + 16*E8(x^2) + 1152*d(x))/17, i.e. b^4[0] = 1, b^4[n] =
 *            (480*sigma_7(n) + 7680*sigma_7(n/2) + 1152*d(n))/17, and d(x) =
 *            x*Product_{k>=1} (1-x^k)^8*(1-x^(2k))^8 (A002288), computed with
 *            the Jacobi triple-product and pentagonal-numbe...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 5, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static int a2272_fixed(void) {
    seq_t z = 0;
    z -= 1;
    return z > 0;
}

/* f <- f * S(x^st), S = prod(1-x^k)^3 (pent == 0) or prod(1-x^k) (pent != 0) */
static void a2272_sparse(seq_t *f, idx_t L, idx_t st, int pent) {
    for (idx_t i = L; i-- > 0;) {
        seq_t s = f[i];
        if (!pent) {
            for (idx_t m = 1;; m++) {
                idx_t e = st * (m * (m + 1) / 2);
                if (e > i) break;
                seq_t t = (seq_t)(2 * m + 1) * f[i - e];
                if (m & 1) s -= t; else s += t;
            }
        } else {
            for (idx_t k = 1;; k++) {
                idx_t e1 = st * (k * (3 * k - 1) / 2);
                if (e1 > i) break;
                idx_t e2 = e1 + st * k;
                if (k & 1) {
                    s -= f[i - e1];
                    if (e2 <= i) s -= f[i - e2];
                } else {
                    s += f[i - e1];
                    if (e2 <= i) s += f[i - e2];
                }
            }
        }
        f[i] = s;
    }
}

static seq_t *a2272_b4 = NULL;
static seq_t *a2272_dd = NULL;
static idx_t a2272_cap = 0;

static void a2272_ensure(idx_t n) {
    if (n < a2272_cap) return;
    idx_t L = a2272_cap * 2;
    if (L < n + 1) L = n + 1;
    if (L < 64) L = 64;

    seq_t *F = mem_new(seq_t, L);
    F[0] = 1;
    a2272_sparse(F, L, 1, 0);
    a2272_sparse(F, L, 1, 0);
    a2272_sparse(F, L, 1, 1);
    a2272_sparse(F, L, 1, 1);
    a2272_sparse(F, L, 2, 0);
    a2272_sparse(F, L, 2, 0);
    a2272_sparse(F, L, 2, 1);
    a2272_sparse(F, L, 2, 1);

    seq_t *dd = mem_new(seq_t, L);
    for (idx_t i = 1; i < L; i++) dd[i] = F[i - 1];
    mem_free(F);

    seq_t *s7 = mem_new(seq_t, L);
    for (idx_t k = 1; k < L; k++) {
        seq_t p = (seq_t)k;
        seq_t p7 = p * p * p * p * p * p * p;
        for (idx_t m = k; m < L; m += k) s7[m] += p7;
    }

    int fixed = a2272_fixed();
    seq_t inv = 17;
    if (fixed) {
        for (int it = 0; it < 8; it++) inv = inv * ((seq_t)2 - (seq_t)17 * inv);
    }

    seq_t *b4 = mem_new(seq_t, L);
    b4[0] = 1;
    for (idx_t i = 1; i < L; i++) {
        seq_t N = (seq_t)480 * s7[i] + (seq_t)1152 * dd[i];
        if (i % 2 == 0) N += (seq_t)7680 * s7[i / 2];
        if (fixed) b4[i] = N * inv;
        else b4[i] = N / 17;
    }
    mem_free(s7);

    if (a2272_b4) mem_free(a2272_b4);
    if (a2272_dd) mem_free(a2272_dd);
    a2272_b4 = b4;
    a2272_dd = dd;
    a2272_cap = L;
}

seq_t a(idx_t n) {
    a2272_ensure(n);
    const seq_t *b4 = a2272_b4;
    const seq_t *dd = a2272_dd;
    seq_t s = 0;
    seq_t t = 0;
    for (idx_t i = 0; i <= n; i++) {
        s += b4[i] * (b4[n - i] - (seq_t)192 * dd[n - i]);
        t += dd[i] * dd[n - i];
    }
    return s + (seq_t)576 * t;
}
