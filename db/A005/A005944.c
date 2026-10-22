/*
 * A005944  Theta series of 18-dimensional lattice associated with Sp_4(4),
 *          with det 1024 and minimal norm 4.
 * https://oeis.org/A005944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [q^n] ( theta_3(q)^18 - 36*theta_3(q)^14*F(q) +
 *            3456*theta_3(q)^10*F(q)^2 + 15360*theta_3(q)^6*F(q)^3 ), where
 *            theta_3(q) = Sum_{k=-inf..inf} q^(k^2) and F(q) = Sum_{m odd}
 *            sigma(m) q^m = q*psi(q^2)^4, psi(q) = Sum_{k>=0} q^(k(k+1)/2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

/* in place: v <- v * theta_3(q), truncated to len terms */
static void mul_theta(seq_t *v, idx_t len)
{
    idx_t i, k;
    for (i = len; i-- > 0;) {
        seq_t s = 0;
        for (k = 1; k * k <= i; k++) s += v[i - k * k];
        v[i] += s + s;
    }
}

/* in place: v <- v * psi(q^2) = v * Sum_{k>=0} q^(k(k+1)), truncated to len terms */
static void mul_psi2(seq_t *v, idx_t len)
{
    idx_t i, k;
    for (i = len; i-- > 0;) {
        seq_t s = 0;
        for (k = 1; k * (k + 1) <= i; k++) s += v[i - k * (k + 1)];
        v[i] += s;
    }
}

static seq_t *compute(idx_t L)
{
    seq_t *A = alloc_zero(L);
    seq_t *B = alloc_zero(L);
    seq_t *W = alloc_zero(L);
    idx_t i;
    int r;

    /* A = theta^8 */
    A[0] = 1;
    for (r = 0; r < 8; r++) mul_theta(A, L);

    /* B = 36 * theta^14 * F  (negative part) */
    for (i = 0; i < L; i++) B[i] = A[i];
    for (r = 0; r < 4; r++) mul_psi2(B, L);
    for (i = L; i-- > 1;) B[i] = B[i - 1] * 36;
    B[0] = 0;
    for (r = 0; r < 6; r++) mul_theta(B, L);

    /* W = psi(q^2)^8, so F^2 = q^2 * W */
    W[0] = 1;
    for (r = 0; r < 8; r++) mul_psi2(W, L);
    for (i = 2; i < L; i++) A[i] += W[i - 2] * 3456;
    for (r = 0; r < 4; r++) mul_theta(A, L);
    /* W = psi(q^2)^12, so F^3 = q^3 * W */
    for (r = 0; r < 4; r++) mul_psi2(W, L);
    for (i = 3; i < L; i++) A[i] += W[i - 3] * 15360;
    for (r = 0; r < 6; r++) mul_theta(A, L);

    for (i = 0; i < L; i++) A[i] -= B[i];

    release(B, L);
    release(W, L);
    return A;
}

seq_t a(idx_t n)
{
    static seq_t *res = NULL;
    static idx_t have = 0;
    if (n >= have) {
        idx_t cap = have + have / 4;
        if (cap < n + 1) cap = n + 1;
        if (cap < 32) cap = 32;
        {
            seq_t *nr = compute(cap);
            if (res) release(res, have);
            res = nr;
            have = cap;
        }
    }
    return res[n];
}
