/*
 * A067398  Squares in OR-numbral arithmetic.
 * https://oeis.org/A067398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n | (n << i) for each i such that the i-th bit of n is 1
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* OR all shifts of n by positions of its set bits.  */
    for (idx_t i = 0; i < (sizeof(idx_t) * 8); ++i)
    {
        if ((n >> i) & 1)
            result |= ((seq_t)n << i);
    }
    return result;
}
