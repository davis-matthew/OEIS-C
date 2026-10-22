/*
 * A004535  Theta series of 14-dimensional unimodular lattice (E7+E7)+.
 * https://oeis.org/A004535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = (theta3^7 + 7*theta3^3*theta2^4)^2 + (theta2^7 +
 *            7*theta2^3*theta3^4)^2, with q^n counting vectors of norm n.
 *            Substituting theta3 = sum_k q^(2k^2) and theta2 = 2 q^(1/2) T
 *            where T = sum_{k>=0} q^(2k(k+1)): P = th3^7 + 112 q^2 th3^3 T^4,
 *            B = 56 th3^4 T^3 + 128 q^2 T^7, a(n) = [q^n] P^2 + [q...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *P_ser = NULL, *B_ser = NULL;
static idx_t cap = 0;

/* multiply by theta3 = 1 + 2*sum_{k>=1} q^(2k^2); consumes s */
static seq_t *mul_th3(seq_t *s, idx_t L) {
    seq_t *out = alloc_zero(L);
    for (idx_t i = 0; i < L; i++) {
        seq_t acc = s[i];
        for (idx_t k = 1; 2 * k * k <= i; k++) {
            seq_t t = s[i - 2 * k * k];
            acc += t;
            acc += t;
        }
        out[i] = acc;
    }
    release(s, L);
    return out;
}

/* multiply by T = sum_{k>=0} q^(2k(k+1)); consumes s */
static seq_t *mul_T(seq_t *s, idx_t L) {
    seq_t *out = alloc_zero(L);
    for (idx_t i = 0; i < L; i++) {
        seq_t acc = 0;
        for (idx_t k = 0; 2 * k * (k + 1) <= i; k++)
            acc += s[i - 2 * k * (k + 1)];
        out[i] = acc;
    }
    release(s, L);
    return out;
}

static seq_t *copy_ser(const seq_t *s, idx_t L) {
    seq_t *out = alloc_zero(L);
    for (idx_t i = 0; i < L; i++) out[i] = s[i];
    return out;
}

static void build(idx_t L) {
    seq_t *one = alloc_zero(L);
    one[0] = 1;
    seq_t *t3_3 = mul_th3(mul_th3(mul_th3(one, L), L), L);
    seq_t *t3_4 = mul_th3(copy_ser(t3_3, L), L);
    seq_t *t3_7 = mul_th3(mul_th3(mul_th3(copy_ser(t3_4, L), L), L), L);
    seq_t *X = t3_3; /* th3^3 T^4 */
    for (int r = 0; r < 4; r++) X = mul_T(X, L);
    seq_t *Y = t3_4; /* th3^4 T^3 */
    for (int r = 0; r < 3; r++) Y = mul_T(Y, L);
    seq_t *T7 = alloc_zero(L);
    T7[0] = 1;
    for (int r = 0; r < 7; r++) T7 = mul_T(T7, L);

    seq_t *P = alloc_zero(L);
    seq_t *B = alloc_zero(L);
    for (idx_t i = 0; i < L; i++) {
        P[i] = t3_7[i];
        B[i] = (seq_t)56 * Y[i];
        if (i >= 2) {
            P[i] += (seq_t)112 * X[i - 2];
            B[i] += (seq_t)128 * T7[i - 2];
        }
    }
    release(t3_7, L);
    release(X, L);
    release(Y, L);
    release(T7, L);

    if (P_ser) release(P_ser, cap);
    if (B_ser) release(B_ser, cap);
    P_ser = P;
    B_ser = B;
    cap = L;
}

seq_t a(idx_t n) {
    if (n >= cap) {
        idx_t L = cap * 2;
        if (L < n + 1) L = n + 1;
        if (L < 64) L = 64;
        build(L);
    }
    seq_t res = 0;
    for (idx_t i = 0; i <= n; i++)
        res += P_ser[i] * P_ser[n - i];
    if (n >= 3) {
        idx_t m = n - 3;
        for (idx_t i = 0; i <= m; i++)
            res += B_ser[i] * B_ser[m - i];
    }
    return res;
}
