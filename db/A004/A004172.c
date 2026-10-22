/*
 * A004172  Triangle of coefficients of Euler polynomials E_2n(x) (exponents
 *          in increasing order).
 * https://oeis.org/A004172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, nice
 * Formula:   T(r,j) = coefficient of x^j in E_{2r}(x) =
 *            binomial(2r,k)*E_k(0), k = 2r-j, where E_0(0) = 1, E_k(0) = 0
 *            for even k > 0, and E_k(0) = (-1)^((k+1)/2)*G_{k+1}/(k+1) for
 *            odd k (G = Genocchi numbers 1,1,3,17,155,..., from the Seidel
 *            triangle). a(n) = T(r,j) with r = floor(sqrt(n)), j = n - r^2
 * Range:     i8 n <= 25, i16 n <= 49, i32 n <= 121, i64 n <= 256,
 *            i128 n <= 625, big any n
 * Helpers:   alloc_zero, binom_s, gcd_s, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/binom_s.h"
#include "../utils/gcd_s.h"

/* Unsigned Genocchi number G_{2t} (t >= 1) via the Seidel triangle;
   all intermediate entries are <= the result. */
static seq_t genocchi(idx_t t) {
    seq_t *r = alloc_zero(t + 1);
    idx_t L = 1, i;
    seq_t res;
    r[0] = 1;
    while (L < t) {
        /* decreasing pass on row of length L */
        for (i = L - 1; i > 0; i--)
            r[i - 1] += r[i];
        /* increasing pass, row grows to length L+1 */
        for (i = 1; i < L; i++)
            r[i] += r[i - 1];
        r[L] = r[L - 1];
        L++;
    }
    res = r[t - 1];
    release(r, t + 1);
    return res;
}

seq_t a(idx_t n) {
    idx_t row = 0, m, j, k, t;
    seq_t G, kk, g, p, q, b, res;

    while ((row + 1) * (row + 1) <= n) row++;
    j = n - row * row;
    m = 2 * row;
    k = m - j;

    if (k == 0) return 1;
    if (k % 2 == 0) return 0;

    t = (k + 1) / 2;
    G = genocchi(t);
    kk = (seq_t)(k + 1);
    g = gcd_s(G, kk);
    p = G / g;
    q = kk / g;

    b = binom_s(m, k);
    b /= q;
    res = b * p;
    if (t % 2 == 1) res = -res;
    return res;
}
