/*
 * A198833  The number of inequivalent ways to color the vertices of a regular
 *          octahedron using at most n colors.
 * https://oeis.org/A198833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2+n+2)*(n^2+n+4)/48
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 76, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = T*(T+1)*(T+2)/6 where T = n*(n+1)/2 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t n_s = (seq_t)n;

    /* T = n*(n+1)/2  -- the triangular number */
    seq_t t = n_s * (n_s + 1);
    t /= 2;

    /* a(n) = T*(T+1)*(T+2)/6  */
    seq_t a1 = t;
    seq_t b1 = t + 1;
    seq_t c1 = t + 2;
    seq_t d  = (seq_t)6;

    /* Divide the product by 6 early to avoid overflow on 64‑bit builds */
    seq_t g = gcd(a1, d);
    a1 /= g;
    d   /= g;

    g = gcd(b1, d);
    b1 /= g;
    d   /= g;

    g = gcd(c1, d);
    c1 /= g;
    d   /= g;   /* d should now be 1 */

    seq_t res = a1;
    res *= b1;
    res *= c1;
    return res;
}
