/*
 * A007143  E-trees with exactly 2 colors.
 * https://oeis.org/A007143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007141(n) - 2*A003238(n) for n > 1, a(1) = 0, where f =
 *            A007141: f(1) = 1, f(n) = Sum_{i=0..n-1} g(i)*g(n-1-i), g(0) =
 *            1, g(s) = Sum_{d|s} f(d); h = A003238: h(1) = 1, h(n) =
 *            Sum_{d|n-1} h(d)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/grow_zero.h"

static seq_t *F = 0, *G = 0, *H = 0;
static idx_t cap = 0, done = 0; /* indices 1..done of F,H and 0..done-1 of G valid */

static void ensure(idx_t n)
{
    idx_t m, d, i;
    if (n + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 16) nc = 16;
        F = grow_zero(F, cap, nc);
        G = grow_zero(G, cap, nc);
        H = grow_zero(H, cap, nc);
        cap = nc;
    }
    for (m = done + 1; m <= n; m++) {
        if (m == 1) {
            G[0] = 1;
            F[1] = 1;
            H[1] = 1;
        } else {
            idx_t s = m - 1;
            seq_t gs = 0, hs = 0;
            for (d = 1; d * d <= s; d++) {
                if (s % d == 0) {
                    idx_t e = s / d;
                    gs += F[d];
                    hs += H[d];
                    if (e != d) {
                        gs += F[e];
                        hs += H[e];
                    }
                }
            }
            G[s] = gs;
            H[m] = hs;
            {
                seq_t acc = 0;
                idx_t half = s / 2; /* pairs (i, s-i) with i < s-i */
                for (i = 0; i < (s + 1) / 2; i++)
                    acc += G[i] * G[s - i];
                acc *= 2;
                if (s % 2 == 0)
                    acc += G[half] * G[half];
                F[m] = acc;
            }
        }
    }
    if (n > done) done = n;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    ensure(n);
    return F[n] - 2 * H[n];
}
