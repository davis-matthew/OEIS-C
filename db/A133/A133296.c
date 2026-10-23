/*
 * A133296  Smallest number whose sum of digits is 2n.
 * https://oeis.org/A133296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest number whose sum of digits is 2*n Constructed by
 *            writing digit sum S=2*n as: S = 9*d + r, then a(n) = r * 10^d +
 *            (10^d - 1) (r=0 case gives 10^d-1) Special case: a(0) = 0
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 85, u128 n <= 172,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Compute the digit sum S = 2*n   (use seq_t to avoid overflow) */
    idx_t S = 2 * n;
    idx_t d = S / 9;                     /* number of trailing 9's    */
    idx_t r = S % 9;                     /* leading digit if any      */

    /* Compute 10^d in seq_t */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < d; ++i) {
        pow10 *= (seq_t)10;
    }

    /* Build the answer */
    if (r == 0) {
        /* S divisible by 9: number is d 9's */
        return pow10 - (seq_t)1;
    } else {
        /* Leading digit r followed by d 9's */
        seq_t lead = (seq_t)r * pow10;
        return lead + (pow10 - (seq_t)1);
    }
}
