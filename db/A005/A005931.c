/*
 * A005931  Theta series of the coset of the E_7 lattice in its dual.
 * https://oeis.org/A005931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] ( 56*psi(q^2)^3*phi(q)^4 + 128*q*psi(q^2)^7 ),
 *            phi(q) = Sum_{k} q^(k^2), psi(q) = Sum_{k>=0} q^(k(k+1)/2)
 * Range:     u8 n <= 0, u16 n <= 10, u32 n <= 952, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_zero, mul_phi, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/mul_phi.h"

/* in place: A <- A * psi(q^2) = A * Sum_{k>=0} q^(k(k+1)), truncated to L terms */
static void mul_psi2(seq_t *A, idx_t L) {
    for (idx_t i = L; i-- > 0;) {
        seq_t s = A[i];
        for (idx_t k = 1; k * (k + 1) <= i; k++) s += A[i - k * (k + 1)];
        A[i] = s;
    }
}

seq_t a(idx_t n) {
    static seq_t *res = NULL;
    static idx_t cap = 0;
    if (n >= cap) {
        idx_t L = 2 * (n + 1);
        if (L < 64) L = 64;
        seq_t *A = alloc_zero(L);
        seq_t *B = alloc_zero(L);
        A[0] = 1;
        B[0] = 1;
        for (int r = 0; r < 3; r++) mul_psi2(A, L);
        for (int r = 0; r < 4; r++) mul_phi(A, L);
        for (int r = 0; r < 7; r++) mul_psi2(B, L);
        seq_t *R = alloc_zero(L);
        for (idx_t i = 0; i < L; i++) {
            seq_t v = A[i] * 56;
            if (i >= 1) v += B[i - 1] * 128;
            R[i] = v;
        }
        release(A, L);
        release(B, L);
        if (res) release(res, cap);
        res = R;
        cap = L;
    }
    return res[n];
}
