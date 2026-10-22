/*
 * A007990  Poincaré series [or Poincare series] of Lie algebra associated
 *          with a certain braid group.
 * https://oeis.org/A007990
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x^2*(3 - 3*x^2 + 6*x^3 + 7*x^4 - 8*x^5 - 6*x^6 - 4*x^7 +
 *            13*x^8 + 8*x^9 - 8*x^11 - 14*x^12 + 6*x^13 + 6*x^14 + 6*x^15 -
 *            3*x^16 - 6*x^17 + 3*x^18) /
 *            ((1-x^4)*(1-x^3)^2*(1-x^2)^4*(1-x)^2); a(n) = Sum_{j=0..18} N_j
 *            * G(n-2-j), where G(m) = [x^m]
 *            1/((1-x^4)*(1-x^3)^2*(1-x^2)^4*(1-x)^2)
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 100, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *G = 0;
static idx_t Glen = 0;

static void ensure_G(idx_t m) {
    static const int ks[9] = {1, 1, 2, 2, 2, 2, 3, 3, 4};
    idx_t newlen, i;
    int f;
    if (m < Glen) return;
    newlen = m + 1;
    if (newlen < 2 * Glen) newlen = 2 * Glen;
    if (newlen < 64) newlen = 64;
    if (G) release(G, Glen);
    G = alloc_zero(newlen);
    G[0] = 1;
    for (f = 0; f < 9; f++) {
        idx_t k = (idx_t)ks[f];
        for (i = k; i < newlen; i++) G[i] += G[i - k];
    }
    Glen = newlen;
}

seq_t a(idx_t n) {
    static const int q[19] = {
        3, 0, -3, 6, 7, -8, -6, -4, 13, 8, 0, -8, -14, 6, 6, 6, -3, -6, 3
    };
    seq_t pos = 0, neg = 0;
    idx_t m, j;
    if (n < 2) return 0;
    m = n - 2;
    ensure_G(m);
    for (j = 0; j <= 18 && j <= m; j++) {
        int c = q[j];
        if (c > 0) pos += (seq_t)c * G[m - j];
        else if (c < 0) neg += (seq_t)(-c) * G[m - j];
    }
    return pos - neg;
}
