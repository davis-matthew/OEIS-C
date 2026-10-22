/*
 * A005768  Number of convex polygons of length 2n on square lattice whose
 *          leftmost bottom vertex is strictly to the left of the rightmost
 *          top vertex.
 * https://oeis.org/A005768
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x^2*(1-8x+21x^2-19x^3+4x^4)/((1-4x)^2*(1-2x)) -
 *            2*x^4*(1-4x)^(-3/2), i.e. a(n) = r(n) - 2*(2m+1)*binomial(2m,m)
 *            with m = n-4 (second term only for n >= 4), where r(n) =
 *            10*r(n-1) - 32*r(n-2) + 32*r(n-3) + p(n-2), p = [1,-8,21,-19,4],
 *            r(n) = 0 for n < 2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const int pos[5] = {1, 0, 21, 0, 4};
    static const int neg[5] = {0, 8, 0, 19, 0};
    seq_t r1 = 0, r2 = 0, r3 = 0; /* r(k-1), r(k-2), r(k-3) */
    seq_t r = 0;
    idx_t k, i;

    for (k = 2; k <= n; k++) {
        idx_t j = k - 2;
        r = r1 * 10 + r3 * 32;
        if (j < 5) r += pos[j];
        r -= r2 * 32;
        if (j < 5) r -= neg[j];
        r3 = r2;
        r2 = r1;
        r1 = r;
    }

    if (n >= 4) {
        idx_t m = n - 4;
        seq_t c = 1; /* binomial(2i, i) */
        seq_t b;
        for (i = 0; i < m; i++) {
            c = (c * 2) / (seq_t)(i + 1);
            c = c * (seq_t)(2 * i + 1);
        }
        b = c * (seq_t)(2 * m + 1);
        r -= b * 2;
    }
    return r;
}
