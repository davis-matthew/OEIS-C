/*
 * A005738  Number of degenerate fanout-free Boolean functions of n variables
 *          using And, Or, Xor, and Not gates.
 * https://oeis.org/A005738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n,k) * A005616(k), where
 *            A005616(0) = 2 and, for n >= 1, A005616(n) = 2*(V(n) + A(n) +
 *            X(n)) with V(n) = [n == 1] and EGFs A = exp(S) - 1 - S, X =
 *            exp(U) - 1 - U, S = 2V + 2X + A, U = V + A (A = AND/OR-rooted, X
 *            = XOR-rooted read-once function classes up to complement)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_zero.h"

static seq_t *tA = 0, *tX = 0, *tF = 0, *tG = 0, *tS = 0, *tU = 0, *tB = 0;
static idx_t have = 0, cap = 0;

static void ensure(idx_t m) {
    idx_t i, k;
    if (m < have) return;
    if (m + 1 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < m + 1) nc *= 2;
        tA = grow_zero(tA, cap, nc);
        tX = grow_zero(tX, cap, nc);
        tF = grow_zero(tF, cap, nc);
        tG = grow_zero(tG, cap, nc);
        tS = grow_zero(tS, cap, nc);
        tU = grow_zero(tU, cap, nc);
        tB = grow_zero(tB, cap, nc);
        cap = nc;
    }
    for (i = have; i <= m; i++) {
        if (i == 0) {
            tA[0] = 0; tX[0] = 0; tS[0] = 0; tU[0] = 0;
            tF[0] = 1; tG[0] = 1; tB[0] = 2;
            continue;
        }
        {
            seq_t v = (i == 1) ? (seq_t)1 : (seq_t)0;
            seq_t sa = 0, sx = 0;
            seq_t c = 1; /* C(i-1, k-1) */
            for (k = 1; k < i; k++) {
                sa += c * tS[k] * tF[i - k];
                sx += c * tU[k] * tG[i - k];
                c = c * (seq_t)(i - k) / (seq_t)k;
            }
            tA[i] = sa;
            tX[i] = sx;
            tS[i] = v * 2 + sx * 2 + sa;
            tU[i] = v + sa;
            tF[i] = sa + tS[i];
            tG[i] = sx + tU[i];
            tB[i] = (v + sa + sx) * 2;
        }
    }
    have = m + 1;
}

seq_t a(idx_t n) {
    seq_t r = 0, c = 1; /* C(n, k) */
    idx_t k;
    if (n == 0) return 0;
    ensure(n - 1);
    for (k = 0; k < n; k++) {
        r += c * tB[k];
        c = c * (seq_t)(n - k) / (seq_t)(k + 1);
    }
    return r;
}
