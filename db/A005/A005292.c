/*
 * A005292  Representation degeneracies for boson strings.
 * https://oeis.org/A005292
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} (-1)^(k+1) * (B(n-k(k+3)/2) - 2*B(n-k(k+5)/2)
 *            + B(n-k(k+7)/2)), where B = A000712 (g.f.
 *            1/Product_{k>=1}(1-x^k)^2); equivalently G.f.: Sum_{k>=1}
 *            (-1)^(k+1) x^(k(k+3)/2) (1-x^k)^2 / Product_{k>=1}(1-x^k)^2
 *            (multiplicity of spin 2 in the light-cone Fock space of 2
 *            transverse osci...
 * Range:     u8 n <= 19, u16 n <= 41, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *P = NULL;   /* partition numbers A000041 */
static seq_t *B = NULL;   /* A000712 */
static idx_t cap = 0;

static void ensure(idx_t need)
{
    idx_t newcap, m, k;
    if (need <= cap) return;
    newcap = 2 * cap;
    if (newcap < need) newcap = need;
    P = grow_zero(P, cap, newcap);
    B = grow_zero(B, cap, newcap);
    for (m = cap; m < newcap; m++) {
        seq_t pp = 0, pn = 0, bp = 0, bn = 0;
        if (m == 0) {
            P[0] = 1;
            B[0] = 1;
            continue;
        }
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > m) break;
            if (k & 1) {
                pp += P[m - g1];
                bp += B[m - g1];
                if (g2 <= m) { pp += P[m - g2]; bp += B[m - g2]; }
            } else {
                pn += P[m - g1];
                bn += B[m - g1];
                if (g2 <= m) { pn += P[m - g2]; bn += B[m - g2]; }
            }
        }
        P[m] = pp - pn;
        B[m] = bp - bn + P[m];
    }
    cap = newcap;
}

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    idx_t k;
    ensure(n + 1);
    for (k = 1; k * (k + 3) / 2 <= n; k++) {
        idx_t d1 = k * (k + 3) / 2;
        idx_t d2 = k * (k + 5) / 2;
        idx_t d3 = k * (k + 7) / 2;
        if (k & 1) {
            pos += B[n - d1];
            if (d2 <= n) neg += 2 * B[n - d2];
            if (d3 <= n) pos += B[n - d3];
        } else {
            neg += B[n - d1];
            if (d2 <= n) pos += 2 * B[n - d2];
            if (d3 <= n) neg += B[n - d3];
        }
    }
    return pos - neg;
}
