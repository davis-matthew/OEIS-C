/*
 * A002490  Theta series of 27-dimensional unimodular lattice with root system
 *          A_1 and a parity vector of norm 3.
 * https://oeis.org/A002490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = theta3^27 - 54*theta3^19*delta8 + 218*theta3^11*delta8^2
 *            - 1024*theta3^3*delta8^3, theta3 = A000122, delta8 = A002408.
 *            Uses delta8 = h*(theta3^4 - 16*h), h = theta2^4/16 =
 *            q*psi(q^2)^4, psi(q) = Sum_{k>=0} q^(k(k+1)/2), so the g.f. is
 *            Sum_{i=0..6} e_i * theta3^(27-4i) * h^i with e = 1, -54...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_zero, mulPsi2, mulT, mulc, mulh2, shiftq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/mulPsi2.h"
#include "../utils/mulT.h"
#include "../utils/mulc.h"
#include "../utils/mulh2.h"
#include "../utils/shiftq.h"

static seq_t *A_memo = 0;
static idx_t A_cap = 0;

static void compute(idx_t L)
{
    seq_t *P = alloc_zero(L);
    seq_t *Tp[7];
    seq_t *E, *O;
    idx_t m;
    int k, i;

    P[0] = 1;
    for (i = 0; i < 3; i++) mulT(P, L);
    for (k = 0; k < 7; k++) {
        Tp[k] = alloc_zero(L);
        for (m = 0; m < L; m++) Tp[k][m] = P[m];
        if (k < 6) for (i = 0; i < 4; i++) mulT(P, L);
    }
    /* Tp[k] = theta3^(3+4k) */

    E = alloc_zero(L);
    for (m = 0; m < L; m++) E[m] = mulc(Tp[0][m], 4194304UL);
    mulh2(E, L);
    for (m = 0; m < L; m++) E[m] += mulc(Tp[2][m], 104960UL);
    mulh2(E, L);
    for (m = 0; m < L; m++) E[m] += mulc(Tp[4][m], 1082UL);
    mulh2(E, L);
    for (m = 0; m < L; m++) E[m] += Tp[6][m];

    O = alloc_zero(L);
    for (m = 0; m < L; m++) O[m] = mulc(Tp[1][m], 786432UL);
    mulh2(O, L);
    for (m = 0; m < L; m++) O[m] += mulc(Tp[3][m], 8000UL);
    mulh2(O, L);
    for (m = 0; m < L; m++) O[m] += mulc(Tp[5][m], 54UL);
    for (i = 0; i < 4; i++) mulPsi2(O, L);
    shiftq(O, L, 1);

    for (m = 0; m < L; m++) E[m] = E[m] - O[m];

    A_memo = E;
    A_cap = L;
}

seq_t a(idx_t n)
{
    if (n >= A_cap) {
        idx_t L = n + n / 2 + 64;
        idx_t g = A_cap + A_cap / 2;
        if (g > L) L = g;
        compute(L);
    }
    return A_memo[n];
}
