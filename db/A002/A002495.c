/*
 * A002495  Theta series of 27-dimensional unimodular lattice with root system
 *          A_1 with no parity vector of norm 3.
 * https://oeis.org/A002495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: theta3^27 - 54*theta3^19*delta8 + 218*theta3^11*delta8^2,
 *            rewritten with A = theta3^4, B = theta2^4/16 = q*psi(q^2)^4,
 *            delta8 = B*(A - 16*B): G.f. = theta3^11 * (A^4 - 54*A^3*B +
 *            1082*A^2*B^2 - 6976*A*B^3 + 55808*B^4)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_zero, mul_A, mul_B, psi_pass, th_pass
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/mul_A.h"
#include "../utils/mul_B.h"
#include "../utils/psi_pass.h"
#include "../utils/th_pass.h"

seq_t a(idx_t n) {
    static seq_t *res = NULL;
    static idx_t have = 0;
    if (n >= have) {
        idx_t N = n + n / 4 + 16;
        idx_t L = N + 1;
        seq_t *X = alloc_zero(L), *T = alloc_zero(L);
        seq_t *P = alloc_zero(L), *Q = alloc_zero(L);
        idx_t m;
        int i;

        X[0] = 1;
        for (i = 0; i < 11; i++) th_pass(&X, &T, L);   /* E0 = theta3^11 */
        for (m = 0; m < L; m++) P[m] = X[m];

        /* s1 = A*s0 - 54*E1 */
        mul_A(&P, &T, L);
        mul_B(&X, &T, L);
        for (m = 0; m < L; m++) Q[m] = X[m] * 54;

        /* s2 = A*s1 + 1082*E2 */
        mul_B(&X, &T, L);
        mul_A(&P, &T, L);
        mul_A(&Q, &T, L);
        for (m = 0; m < L; m++) P[m] += X[m] * 1082;

        /* s3 = A*s2 - 6976*E3 */
        mul_B(&X, &T, L);
        mul_A(&P, &T, L);
        mul_A(&Q, &T, L);
        for (m = 0; m < L; m++) Q[m] += X[m] * 6976;

        /* s4 = A*s3 + 55808*E4 */
        mul_B(&X, &T, L);
        mul_A(&P, &T, L);
        mul_A(&Q, &T, L);
        for (m = 0; m < L; m++) P[m] += X[m] * 55808;

        for (m = 0; m < L; m++) P[m] -= Q[m];

        res = P;
        have = L;
    }
    return res[n];
}
