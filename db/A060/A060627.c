/*
 * A060627  1 + Sum_{n >= 1} Sum_{k = 0..n-1}
 *          (-1)^n*T(n,k)*y^(2*k)*x^(2*n)/(2*n)! = JacobiCN(x,y).
 * https://oeis.org/A060627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = the (n‑th) element of the triangle T(n,k) defined by the
 *            recurrence B_0(x) = 1, B_{m+1}(x) = B_m(x) + 2x (S_m(x) –
 *            B_m(x)) where S_m(x) = sum_{i+j+k=m} ( (2m)! / ((2i)! (2j)!
 *            (2k)!) ) B_i(x)B_j(x)B_k(x) and B_m(x) = sum_{k=0}^{m-1} T(m,k)
 *            x^k , (m≥1), B_0(x)=1. The sequence a(n) is obtaine...
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 31, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A060627: triangle T(n,k), n >= 1, k = 0..n-1: the coefficient polynomials
 * of the Jacobi cn expansion, from the continued fraction (Peter Bala, after
 * Wall) 1/(1 - x/(1 - (2k)^2 x/(1 - 3^2 x/(1 - (4k)^2 x/(1 - ...))))):
 * the coefficient of x^n is the sum over Dyck paths of semilength n of the
 * product of the weights of their up steps, an up step to height h weighing
 * h^2 for odd h and h^2 K for even h (K = k^2). A dynamic program over
 * (step, height) with polynomials in K gives row n; sums and products only,
 * so exact modulo the width. (The original wrote into zero-length arrays.) */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    size_t row = 1, idx = (size_t)n - 1;              /* row r has r entries */
    while (idx >= row) { idx -= row; ++row; }
    const size_t N = row, H = N + 1, D = N + 1;       /* heights 0..N, degrees 0..N */
    /* P[h * D + d]: paths so far ending at height h, coefficient of K^d */
    seq_t *P = mem_new(seq_t, H * D), *Q = mem_new(seq_t, H * D);
    if (!P || !Q) abort();
    P[0] = (seq_t)1;
    for (size_t s = 0; s < 2 * N; ++s) {
        for (size_t i = 0; i < H * D; ++i) Q[i] = (seq_t)0;
        for (size_t h = 0; h < H; ++h)
            for (size_t d = 0; d < D; ++d) {
                seq_t v = P[h * D + d];
                if (v == (seq_t)0) continue;
                if (h + 1 < H) {                       /* up to h+1, weighted */
                    size_t u = h + 1, nd = d + (u % 2 == 0);
                    if (nd < D) Q[u * D + nd] += v * (seq_t)(u * u);
                }
                if (h > 0) Q[(h - 1) * D + d] += v;    /* down */
            }
        seq_t *t = P; P = Q; Q = t;
    }
    seq_t r = idx < D ? P[idx] : (seq_t)0;            /* height 0, K^idx */
    mem_free(P);
    mem_free(Q);
    return r;
}
