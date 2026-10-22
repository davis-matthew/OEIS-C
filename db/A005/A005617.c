/*
 * A005617  Number of non-degenerate fanout-free Boolean functions of n
 *          variables using And, Or, Xor, Not, and Majority gates.
 * https://oeis.org/A005617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. 2 + 2*A(x), A = Series_Reversion(log(1+3x+2x^2) - 2x -
 *            2x^3/3). A satisfies A' = (1+3A+2A^2)/(1-2A-6A^2-6A^3-4A^4),
 *            i.e. A' = 1 + 3A + 2A^2 + A'*(2A + 6A^2 + 6A^3 + 4A^4), A(0) =
 *            0. With b(n) = n![x^n]A (EGF products are binomial
 *            convolutions): b(n+1) = [n=0] + 3b(n) + 2P2(n) + Sum_{k=1..n...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *B = 0, *P2 = 0, *P3 = 0, *P4 = 0, *W = 0, *C = 0;
static idx_t cap = 0;
static idx_t have = 0; /* B,P2,P3,P4,W known for indices 0..have; C holds row 'have' */

static void ensure(idx_t need) {
    if (need <= cap) return;
    idx_t nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    if (cap == 0) {
        B = alloc_zero(nc); P2 = alloc_zero(nc); P3 = alloc_zero(nc);
        P4 = alloc_zero(nc); W = alloc_zero(nc); C = alloc_zero(nc);
        C[0] = 1;
    } else {
        B = grow_zero(B, cap, nc); P2 = grow_zero(P2, cap, nc);
        P3 = grow_zero(P3, cap, nc); P4 = grow_zero(P4, cap, nc);
        W = grow_zero(W, cap, nc); C = grow_zero(C, cap, nc);
    }
    cap = nc;
}

static void step(void) {
    idx_t m = have, k;
    ensure(m + 2);
    /* b(m+1) using row m of binomials */
    seq_t s = 0;
    if (m == 0) s += 1;
    s += 3 * B[m];
    s += 2 * P2[m];
    for (k = 1; k <= m; k++)
        s += C[k] * W[k] * B[m - k + 1];
    B[m + 1] = s;
    /* advance binomial row to m+1 */
    C[m + 1] = 1;
    for (k = m; k >= 1; k--) C[k] += C[k - 1];
    idx_t n = m + 1;
    seq_t q2 = 0, q3 = 0, q4 = 0;
    for (k = 1; k < n; k++) q2 += C[k] * B[k] * B[n - k];
    P2[n] = q2;
    for (k = 1; k < n; k++) q3 += C[k] * B[k] * P2[n - k];
    P3[n] = q3;
    for (k = 1; k < n; k++) q4 += C[k] * B[k] * P3[n - k];
    P4[n] = q4;
    W[n] = 2 * B[n] + 6 * P2[n] + 6 * P3[n] + 4 * P4[n];
    have = n;
}

seq_t a(idx_t n) {
    if (n == 0) return 2;
    ensure(2);
    while (have < n) step();
    return 2 * B[n];
}
