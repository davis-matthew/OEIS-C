/*
 * A005740  Number of degenerate fanout-free Boolean functions of n variables
 *          using And, Or, Xor, Not, and Majority gates.
 * https://oeis.org/A005740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n,k) * A005617(k), A005617(0) =
 *            2. A005617(m) = F(m) counts read-once functions by the gate at
 *            the top of the formula: F = 2*A + X + M, where A(m) = Sum over
 *            set partitions into >= 2 blocks of Prod (F - A)(block) (AND
 *            type; OR type is its complement), X(m) = 2 * Sum...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   binom, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/grow_zero.h"

static seq_t *F = 0, *Fh = 0, *Ad = 0, *U = 0, *Xh = 0, *V = 0, *P2 = 0, *row = 0;
static idx_t cnt = 0;   /* F[1..cnt] are computed; row holds binomial row cnt */
static idx_t cap = 0;

static void ensure(idx_t n) {
    if (n + 2 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < n + 2) nc *= 2;
        F = grow_zero(F, cap, nc);
        Fh = grow_zero(Fh, cap, nc);
        Ad = grow_zero(Ad, cap, nc);
        U = grow_zero(U, cap, nc);
        Xh = grow_zero(Xh, cap, nc);
        V = grow_zero(V, cap, nc);
        P2 = grow_zero(P2, cap, nc);
        row = grow_zero(row, cap, nc);
        if (cap == 0) row[0] = 1;
        cap = nc;
    }
    while (cnt < n) {
        idx_t m = cnt + 1;
        /* row currently holds binomial(m-1, .) */
        if (m == 1) {
            F[1] = 2; Fh[1] = 1; Ad[1] = 0; U[1] = 2;
            Xh[1] = 0; V[1] = 1; P2[1] = 0;
        } else {
            seq_t sa = 0, sx = 0, sp = 0, sm = 0;
            for (idx_t j = 1; j <= m - 1; j++) {
                seq_t c = row[j - 1];
                sa += c * U[j] * F[m - j];
                sx += c * V[j] * Fh[m - j];
                sp += c * F[j] * F[m - j];
                if (j <= m - 2) sm += c * Fh[j] * P2[m - j];
            }
            Ad[m] = sa;
            Xh[m] = sx;
            P2[m] = sp;
            Fh[m] = sa + sx + sm;
            F[m] = Fh[m] * 2;
            U[m] = F[m] - sa;
            V[m] = Fh[m] - sx;
        }
        /* advance row to binomial(m, .) */
        for (idx_t i = m; i >= 1; i--) row[i] += row[i - 1];
        cnt = m;
    }
}

seq_t a(idx_t n) {
    seq_t result = 2;   /* k = 0 term: binomial(n,0) * 2 */
    if (n >= 2) ensure(n - 1);
    for (idx_t k = 1; k + 1 <= n; k++) {
        result += binom(n, k) * F[k];
    }
    return result;
}
