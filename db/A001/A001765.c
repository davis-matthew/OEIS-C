/*
 * A001765  Coefficients of iterated exponentials.
 * https://oeis.org/A001765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   E.g.f. f_7 where f_0 = x, f_{k+1} = -log(1 - f_k). Since
 *            f_{k+1}' = f_k' / (1 - f_k), taking h = f_{k+1}' gives, with
 *            e.g.f. coefficients: f_{k+1}(m+1) = f_k(m+1) + Sum_{i=1..m}
 *            binomial(m,i) * f_k(i) * f_{k+1}(m-i+1), with f_k(0) = 0, f_0(1)
 *            = 1 and f_0(j) = 0 for j != 1; a(n) = f_7(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

#define A1765_LAYERS 7

static seq_t *a1765_F[A1765_LAYERS + 1];
static seq_t *a1765_row;
static idx_t a1765_cap = 0;
static idx_t a1765_have = 0;

static void a1765_ensure(idx_t need) {
    idx_t newcap;
    int k;
    if (need <= a1765_cap) return;
    newcap = a1765_cap ? a1765_cap : 64;
    while (newcap < need) newcap *= 2;
    for (k = 0; k <= A1765_LAYERS; k++)
        a1765_F[k] = grow_zero(a1765_F[k], a1765_cap, newcap);
    a1765_row = grow_zero(a1765_row, a1765_cap, newcap);
    if (a1765_cap == 0) {
        a1765_row[0] = 1; /* binomial row for M = 0 */
        a1765_have = 0;
    }
    a1765_cap = newcap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    a1765_ensure(n + 2);
    while (a1765_have < n) {
        idx_t M = a1765_have;
        idx_t i;
        int k;
        /* row currently holds binomial(M, i) */
        a1765_F[0][M + 1] = (M == 0) ? 1 : 0;
        for (k = 1; k <= A1765_LAYERS; k++) {
            const seq_t *f = a1765_F[k - 1];
            seq_t *g = a1765_F[k];
            seq_t v = f[M + 1];
            for (i = 1; i <= M; i++)
                v += a1765_row[i] * f[i] * g[M - i + 1];
            g[M + 1] = v;
        }
        /* advance binomial row to M + 1 */
        a1765_row[M + 1] = 1;
        for (i = M; i >= 1; i--)
            a1765_row[i] += a1765_row[i - 1];
        a1765_have = M + 1;
    }
    return a1765_F[A1765_LAYERS][n];
}
