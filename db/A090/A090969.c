/*
 * A090969  a(n) = 1/Integral_{x=0..1} (x^5 - x^6)^n.
 * https://oeis.org/A090969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5n+1) * C(6n+1, n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Handle n = 0 directly (m = 1, k = 0) */
    if (n == 0) return 1;

    /* m = 6*n + 1  (the upper term in the binomial),  k = n  */
    idx_t m = 6 * n + 1;
    idx_t k = n;

    seq_t res = 1;

    /* Compute binomial coefficient C(m, k) multiplicatively, cancelling
       factors at each step to keep intermediate values minimal. */
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numerator   = (seq_t)(m - k + i); /* = 5n + 1 + i */
        seq_t denominator = (seq_t)i;

        seq_t g = gcd(numerator, denominator); /* cancel gcd between numerator and denominator */
        numerator   /= g;
        denominator /= g;

        g = gcd(res, denominator);            /* cancel gcd between current result and denominator */
        res      /= g;
        denominator /= g;                     /* after cancellations, denominator should be 1 */

        res *= numerator;                     /* final multiplication */
    }

    /* Multiply by (5n+1) according to the closed form */
    res *= (seq_t)(5 * n + 1);

    return res;
}
