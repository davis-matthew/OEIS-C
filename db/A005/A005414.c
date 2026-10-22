/*
 * A005414  Feynman diagrams of order 2n with vertex skeletons.
 * https://oeis.org/A005414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Gamma(x) = 1/(1 - T(z)) with z = x*Gamma^2*S^2*D, where T(z) =
 *            Sum_{n>=1} a(n) z^n. Zero-dimensional QED with Furry's theorem
 *            gives D = S, S = 1 + x*S^2 + 2*x^2*S' (S = A005411) and Gamma =
 *            (S-1)/(x*S^3) (A005412). Rewritten in z, W = Gamma(z) + z
 *            satisfies (W-1)*W*(W^2 - 2zWW' - 2zW + 3z^2W') =...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   grow_zero, mk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/mk.h"

static seq_t *W = 0, *P = 0, *Bm = 0, *WBm = 0, *Tt = 0;
static unsigned char *Bs = 0, *WBs = 0;
static idx_t done = 0;
static idx_t cap = 0;

static seq_t gam(idx_t i)
{
    return i == 1 ? W[1] - 1 : W[i];
}

seq_t a(idx_t n)
{
    idx_t m, i, k;
    if (cap <= n + 1) {
        idx_t nc = 2 * n + 16;
        W = grow_zero(W, cap, nc);
        P = grow_zero(P, cap, nc);
        Bm = grow_zero(Bm, cap, nc);
        WBm = grow_zero(WBm, cap, nc);
        Tt = grow_zero(Tt, cap, nc);
        Bs = (unsigned char *)realloc(Bs, (size_t)nc);
        WBs = (unsigned char *)realloc(WBs, (size_t)nc);
        cap = nc;
    }
    if (done == 0) {
        W[0] = 1;
        Tt[0] = 0;
    }
    for (m = done + 1; m <= n; m++) {
        idx_t j = m - 1;
        seq_t s, pos, neg, epos, eneg;

        /* P_j = [z^j] W^2 */
        s = 0;
        for (i = 0; i <= j; i++) s += W[i] * W[j - i];
        P[j] = s;

        /* B_j = (1-j) P_j + (3j-5) W_{j-1}, B_0 = P_0 */
        if (j == 0) { Bm[0] = P[0]; Bs[0] = 0; }
        else {
            pos = 0; neg = 0;
            neg += P[j] * (seq_t)(j - 1);
            if (3 * j >= 5) pos += W[j - 1] * (seq_t)(3 * j - 5);
            else neg += W[j - 1] * (seq_t)(5 - 3 * j);
            mk(pos, neg, &Bm[j], &Bs[j]);
        }

        /* WB_j = [z^j] W*B */
        pos = 0; neg = 0;
        for (i = 0; i <= j; i++) {
            seq_t pr = W[i] * Bm[j - i];
            if (Bs[j - i]) neg += pr; else pos += pr;
        }
        mk(pos, neg, &WBm[j], &WBs[j]);

        /* E_j = [z^j] Gamma*(W - zW') = Sum Gamma_i (i+1-j) W_{j-i} */
        epos = 0; eneg = 0;
        for (i = 0; i <= j; i++) {
            if (i + 1 >= j) epos += gam(i) * W[j - i] * (seq_t)(i + 1 - j);
            else eneg += gam(i) * W[j - i] * (seq_t)(j - i - 1);
        }

        /* w_m = 2 E_{m-1} - Sum_{k=1}^{m-1} w_k WB_{m-k} */
        pos = epos * 2; neg = eneg * 2;
        for (k = 1; k < m; k++) {
            seq_t pr = W[k] * WBm[m - k];
            if (WBs[m - k]) pos += pr; else neg += pr;
        }
        W[m] = pos - neg;

        /* t_m = Gamma_m - Sum_{k=1}^{m-1} t_k Gamma_{m-k} */
        s = 0;
        for (k = 1; k < m; k++) s += Tt[k] * gam(m - k);
        Tt[m] = gam(m) - s;
    }
    if (n > done) done = n;
    return Tt[n];
}
