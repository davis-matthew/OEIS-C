/*
 * A002684  Denominators of coefficients for repeated integration.
 * https://oeis.org/A002684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of -(n/2)*M(n) - (2n+2)*M(n+1), where M(n) =
 *            (2/(2n+1)!) * Integral_{t=0..1} t*Product_{k=1..n} (t^2-k^2) dt
 *            = I(n)/(2n+1)!, I(n) = Integral_{u=0..1} Product_{k=1..n}
 *            (u-k^2) du. Then the value is -(n*(2n+3)*I(n) + 2*I(n+1)) /
 *            (2*(2n+3)*(2n+1)!)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   a2684_g, a2684_integ
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a2684_g.h"
#include "../utils/a2684_integ.h"

seq_t a(idx_t n) {
    seq_t n1, d1, n2, d2;
    int s1, s2;
    a2684_integ(n, &n1, &d1, &s1);
    a2684_integ(n + 1, &n2, &d2, &s2);

    /* term1 = n*(2n+3)*I(n) */
    seq_t coef = (seq_t)n * (2 * (seq_t)n + 3);
    seq_t a1, b1;
    if (coef == 0 || n1 == 0) { a1 = 0; b1 = 1; s1 = 0; }
    else {
        seq_t g = a2684_g(coef, d1);
        a1 = n1 * (coef / g);
        b1 = d1 / g;
    }
    /* term2 = 2*I(n+1) */
    seq_t a2, b2;
    if (n2 == 0) { a2 = 0; b2 = 1; s2 = 0; }
    else {
        seq_t g = a2684_g((seq_t)2, d2);
        a2 = n2 * (2 / g);
        b2 = d2 / g;
    }

    seq_t g = a2684_g(b1, b2);
    seq_t B = b1 / g * b2;
    seq_t A1 = a1 * (b2 / g), A2 = a2 * (b1 / g);
    seq_t P;
    if (s1 == s2) P = A1 + A2;
    else P = (A1 >= A2) ? (A1 - A2) : (A2 - A1);
    if (P == 0) return 1;
    seq_t r = a2684_g(P, B);
    seq_t p = P / r, q = B / r;

    seq_t F = 2 * (2 * (seq_t)n + 3);
    for (idx_t i = 2; i <= 2 * n + 1; i++) F *= (seq_t)i;
    seq_t h = a2684_g(p, F);
    return q * (F / h);
}
