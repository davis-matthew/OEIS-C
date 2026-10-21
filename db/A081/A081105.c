/*
 * A081105  5th binomial transform of (1,1,0,0,0,0,.....).
 * https://oeis.org/A081105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+5)*5^(n-1), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast path for the first term */
    if (n == 0)
        return (seq_t)1;

    /* Compute 5^(n-1) by repeated multiplication */
    seq_t pow5 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        pow5 *= (seq_t)5;

    /* Multiply by (n+5) and return */
    return (seq_t)(n + 5) * pow5;
}
