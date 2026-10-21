/*
 * A050815  Number of positive Fibonacci numbers with n decimal digits.
 * https://oeis.org/A050815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=6 ; for n>1, a(n) = 5 if { n*α + β } < { α } else 4, where
 *            α = log10(10)/log10(φ) = 1/log10(φ), β = log10(5)/(2*log10(φ)),
 *            φ = (1+√5)/2, and {x} is the fractional part of x
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case n = 1 */
    if (n == 1ULL)
        return (seq_t)6;

    /* Lazily compute constants once */
    static double alpha = 0.0;
    static double beta  = 0.0;
    static double frac_alpha = 0.0; /* {α} */
    static bool   initialized = false;

    if (!initialized) {
        double phi      = (1.0 + sqrt(5.0)) / 2.0;
        double log10_phi = log10(phi);

        alpha         = 1.0 / log10_phi;                /* α */
        beta          = log10(5.0) / (2.0 * log10_phi); /* β */
        frac_alpha    = alpha - floor(alpha);           /* {α} */
        initialized   = true;
    }

    /* Compute fractional part of n*α + β */
    double val = (double)n * alpha + beta;
    double frac_val = val - floor(val); /* { n*α + β } */

    /* Compare with {α} */
    return (seq_t)(frac_val < frac_alpha ? 5 : 4);
}
