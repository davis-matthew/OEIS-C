/*
 * A130187  Numerators of rationals r(n) related to the z-sequence of the
 *          Sheffer matrix A060821 for Hermite polynomials.
 * https://oeis.org/A130187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of (2*n+1)! / ((n+1)! * 4^n) after cancelling
 *            all factors of 2 from the numerator. This is equivalent to the
 *            sequence defined in OEIS A130187
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* product of integers from n+2 to 2*n+1,
       cancelling powers of two with the denominator 4^n = 2^(2*n). */
    idx_t start = n + 2;          /* first multiplier (>=2 for n>=0) */
    idx_t end   = 2 * n + 1;      /* last multiplier          */
    seq_t result = 1;             /* current product          */
    idx_t denom_power = 2 * n;    /* exponent of 2 in denominator */

    for (idx_t k = start; k <= end; ++k) {
        seq_t val = (seq_t)k;          /* current factor          */
        /* divide out as many factors of 2 as possible,
           but never more than the remaining exponent in the denominator. */
        while ((val % 2 == 0) && denom_power > 0) {
            val /= 2;
            denom_power -= 1;
        }
        result *= val;
    }
    return result;
}
