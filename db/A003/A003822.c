/*
 * A003822  Number of commutative elements in Coxeter group E_n.
 * https://oeis.org/A003822
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: x^3 *
 *            ((16-52*x+45*x^2-x^(-1)*(R(x)-1))/(1-7*x+14*x^2-9*x^3) -
 *            (6-14*x+12*x^2)/(1-4*x+5*x^2-3*x^3) +
 *            (1-x^3-x^4)/(1-x-x^2+x^5)), R(x) = (1-sqrt(1-4*x))/(2*x) the
 *            Catalan g.f. (Stembridge 1998). Each rational part is expanded
 *            by its linear recurrence; Catalan numbers via C(j+1) =
 *            C(j)*(4j+2)...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   a003822_cat_next, a003822_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a003822_cat_next.h"
#include "../utils/a003822_gcd.h"

seq_t a(idx_t n) {
    idx_t m = n - 3, k;
    seq_t c = 1; /* running Catalan number */
    seq_t u1 = 0, u2 = 0, u3 = 0;
    seq_t v1 = 0, v2 = 0, v3 = 0;
    seq_t w1 = 0, w2 = 0, w3 = 0, w4 = 0, w5 = 0;
    seq_t u = 0, v = 0, w = 0;
    for (k = 0; k <= m; k++) {
        seq_t pos, neg;
        c = a003822_cat_next(c, k); /* c = C(k+1) */

        /* u = (16-52x+45x^2 - sum C(k+1)x^k) / (1-7x+14x^2-9x^3) */
        pos = 0; neg = c;
        if (k == 0) pos += 16;
        else if (k == 1) neg += 52;
        else if (k == 2) pos += 45;
        u = u1 * 7 + u3 * 9 + pos;
        u -= u2 * 14;
        u -= neg;

        /* v = (6-14x+12x^2) / (1-4x+5x^2-3x^3) */
        pos = 0; neg = 0;
        if (k == 0) pos += 6;
        else if (k == 1) neg += 14;
        else if (k == 2) pos += 12;
        v = v1 * 4 + v3 * 3 + pos;
        v -= v2 * 5;
        v -= neg;

        /* w = (1-x^3-x^4) / (1-x-x^2+x^5) */
        pos = 0; neg = 0;
        if (k == 0) pos += 1;
        else if (k == 3 || k == 4) neg += 1;
        w = w1 + w2 + pos;
        w -= w5;
        w -= neg;

        u3 = u2; u2 = u1; u1 = u;
        v3 = v2; v2 = v1; v1 = v;
        w5 = w4; w4 = w3; w3 = w2; w2 = w1; w1 = w;
    }
    u += w;
    u -= v;
    return u;
}
