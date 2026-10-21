/*
 * A059616  Numerator of n*(n-1)/(8*(2*n+1)).
 * https://oeis.org/A059616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of n*(n-1)/(8*(2*n+1))
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the numerator of the fraction n*(n-1) / (8*(2*n+1))
       and reduce it by its greatest common divisor with the
       denominator.  All arithmetic is performed in seq_t to
       satisfy the build requirements.  The Euclidean algorithm
       is used for the gcd. */
    seq_t numer = ((seq_t)n) * (((seq_t)n) - 1);
    seq_t denom = 8 * ((((seq_t)2 * (seq_t)n) + 1));

    /* GCD via the Euclidean algorithm */
    seq_t a_tmp = numer, b_tmp = denom;
    while (b_tmp != 0) {
        seq_t r = a_tmp % b_tmp;
        a_tmp = b_tmp;
        b_tmp = r;
    }
    seq_t g = a_tmp;

    return numer / g;
}
