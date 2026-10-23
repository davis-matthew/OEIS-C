/*
 * A017792  Binomial coefficients C(76,n).
 * https://oeis.org/A017792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(76, n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 23, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(76, n) for any non‑negative n.
 * For n > 76 the value is 0.  Uses an efficient multiplicative
 * algorithm that keeps intermediate results integral by cancelling
 * common factors via gcd calculations. */
static inline seq_t _gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* C(76, n) is 0 for n > 76 */
    if (n > 76)
        return 0;

    /* Use symmetry: C(76,n) = C(76, 76-n) */
    idx_t k = n;
    if (k > 76 - k)
        k = 76 - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numerator   = (seq_t)(76 - k + i);     /* (76-k+i) */
        seq_t denominator = (seq_t)i;

        /* Cancel gcd between numerator and denominator */
        seq_t g = _gcd(numerator, denominator);
        numerator   /= g;
        denominator /= g;

        /* Further cancel any remaining gcd between result and denominator */
        g = _gcd(result, denominator);
        result   /= g;
        denominator /= g;

        /* Now denominator should be 1, but if not, divide after multiplication */
        result *= numerator;
        if (denominator != 1)
            result /= denominator;
    }

    return result;
}
