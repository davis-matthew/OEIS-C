/*
 * A071975  Denominator of rational number i/j such that Sagher map sends i/j
 *          to n.
 * https://oeis.org/A071975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy, nice, mult
 * Formula:   a(n) = ∏ pᶠ(eᵢ) where n = ∏ pᵉⁱ and f(e) = (e+1)/2 if e odd, 0
 *            otherwise
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) = ∏ over odd exponents e of p^((e+1)/2) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 1 the factorization is empty, so the denominator is 1. */
    if (n == 1)
        return (seq_t)1;

    seq_t denom = (seq_t)1;
    idx_t x = n;

    /* Trial division over all possible prime factors up to sqrt(x). */
    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p != 0)
            continue;

        /* COUNT exponent of current prime. */
        idx_t exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }

        /* Only odd exponents contribute to the denominator. */
        if (exp & 1) {
            idx_t e_half = (exp + 1) / 2;   /* (exp+1)/2 */

            /* Compute p^e_half in the target integer type. */
            seq_t pow = (seq_t)1;
            for (idx_t k = 0; k < e_half; ++k)
                pow *= (seq_t)p;

            denom *= pow;
        }
    }

    /* If x > 1, it is a prime factor with exponent 1 (odd). */
    if (x > 1) {
        denom *= (seq_t)x;    /* exponent (1+1)/2 = 1 */
    }

    return denom;
}
