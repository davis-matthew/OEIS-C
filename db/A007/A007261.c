/*
 * A007261  McKay-Thompson series of class 6b for the Monster group.
 * https://oeis.org/A007261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] (P(q) + 27*q/P(q)), P(q) = prod_{k>=1}
 *            (1-q^k)^6/(1-q^(3k))^6, i.e. q^(1/2)*(eta(q)^6/eta(q^3)^6 +
 *            27*eta(q^3)^6/eta(q)^6). With H = 1/E(q)^3, E(q)^3 = Sum_{t>=0}
 *            (-1)^t (2t+1) q^(t(t+1)/2) (Jacobi): U = E(q)^3 * H(q^3), S =
 *            E(q^3)^3 * H(q), P = U^2, 1/P = S^2
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 24, u64 n <= 90, u128 n <= 335,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"

static seq_t *Hs = 0, *Ss = 0, *Um = 0;
static bool *Uneg = 0;
static idx_t cap = 0;

static void ensure(idx_t N)
{
    idx_t m, t, T, nc;
    if (N < cap) return;
    nc = cap * 2;
    if (nc < N + 1) nc = N + 1;
    Hs = grow_zero(Hs, cap, nc);
    Ss = grow_zero(Ss, cap, nc);
    Um = grow_zero(Um, cap, nc);
    Uneg = (bool *)realloc(Uneg, (size_t)nc * sizeof(bool));
    for (m = cap; m < nc; m++) {
        seq_t p, q;
        /* H[m] */
        if (m == 0) {
            Hs[m] = 1;
        } else {
            p = 0; q = 0;
            for (t = 1; (T = t * (t + 1) / 2) <= m; t++) {
                seq_t term = Hs[m - T] * (seq_t)(2 * t + 1);
                if (t & 1) p += term; else q += term;
            }
            Hs[m] = p - q;
        }
        /* S[m] = sum (-1)^t (2t+1) H[m - 3T] */
        p = 0; q = 0;
        for (t = 0; 3 * (T = t * (t + 1) / 2) <= m; t++) {
            seq_t term = Hs[m - 3 * T] * (seq_t)(2 * t + 1);
            if (t & 1) q += term; else p += term;
        }
        Ss[m] = p - q;
        /* U[m] = sum (-1)^t (2t+1) H[(m - T)/3] */
        p = 0; q = 0;
        for (t = 0; (T = t * (t + 1) / 2) <= m; t++) {
            if ((m - T) % 3 != 0) continue;
            {
                seq_t term = Hs[(m - T) / 3] * (seq_t)(2 * t + 1);
                if (t & 1) q += term; else p += term;
            }
        }
        if (p >= q) { Um[m] = p - q; Uneg[m] = false; }
        else { Um[m] = q - p; Uneg[m] = true; }
    }
    cap = nc;
}

seq_t a(idx_t n)
{
    seq_t p = 0, q = 0, r = 0;
    idx_t i;
    ensure(n);
    for (i = 0; i <= n; i++) {
        seq_t prod = Um[i] * Um[n - i];
        if (Uneg[i] != Uneg[n - i]) q += prod; else p += prod;
    }
    if (n >= 1) {
        for (i = 0; i <= n - 1; i++) r += Ss[i] * Ss[n - 1 - i];
        p += r * 27;
    }
    return p - q;
}
