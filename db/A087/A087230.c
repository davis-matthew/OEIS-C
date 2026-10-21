/*
 * A087230  a(n) is the 2-adic valuation of 6*n + 4.
 * https://oeis.org/A087230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2-adic valuation of (6*n + 4) i.e. v₂(6*n+4)
 * Range:     u8 n <= 41, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute m = 6*n + 4 in seq_t */
    seq_t m = (seq_t)n * (seq_t)6 + (seq_t)4;

    /* Count the exponent of 2 dividing m */
    seq_t count = (seq_t)0;
    while ((m & (seq_t)1) == (seq_t)0)
    {
        m >>= (int)1;          /* shift right by one bit */
        count = count + (seq_t)1;
    }
    return count;
}
