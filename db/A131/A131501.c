/*
 * A131501  Xm/CV where Xm is a point of maximum error using an approximation
 *          method for x^(1/2) which I have found and CV is the population
 *          coefficient of variation from my list of error values.
 * https://oeis.org/A131501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, obsc, more
 * Formula:   a(n) = 5*n + 1 if n is odd, 5*n if n is even
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, return 0 to stay safe */
    seq_t un = (seq_t)n;
    return 5 * un + ((n & 1ULL) ? 1 : 0);
}
