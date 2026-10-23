/*
 * A022253  Gaussian binomial coefficients [ n,2 ] for q = 9.
 * https://oeis.org/A022253
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((9^n - 1)(9^(n-1) - 1))/640 (Gaussian binomial
 *            coefficient [n,2]_{q=9})
 * Range:     u8 none, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* Undefined for indices below the offset */

    /* Compute the two factors 9^n-1 and 9^(n-1)-1 */
    seq_t pow_n   = big_pow((seq_t)9, (seq_t)n);
    seq_t pow_n_1 = big_pow((seq_t)9, (seq_t)(n - 1));

    seq_t a_coef = pow_n   - (seq_t)1;
    seq_t b_coef = pow_n_1 - (seq_t)1;

    /* Denominator 640 = 2^7 * 5.  We cancel factors with the coefficients first
       to avoid overflow in the product a_coef * b_coef.  The gcd helper works
       for all seq_t types (including arbitrary‑precision). */
    seq_t denom = (seq_t)640, g;

    /* Remove common factors from a_coef and the denominator */
    g = gcd(a_coef, denom);
    a_coef /= g;
    denom   /= g;

    /* Remove common factors from b_coef and the remaining denominator */
    g = gcd(b_coef, denom);
    b_coef /= g;
    denom   /= g;

    /* The remaining denominator should now be 1 (exact division). */
    /* If not, cancel any remaining common factor (unlikely). */
    if (denom > 1) {
        g = gcd(a_coef, denom);
        a_coef /= g;
        denom   /= g;
        g = gcd(b_coef, denom);
        b_coef /= g;
        denom   /= g;
    }

    /* The product now fits in seq_t for all n that produce a result fitting
       in seq_t (e.g., 64‑bit builds).  For larger results seq_t will be
       an arbitrary precision type. */
    return a_coef * b_coef;
}
