/*
 * A079326  a(n) = the largest number m such that if m monominoes are removed
 *          from an n X n square then an L-tromino must remain.
 * https://oeis.org/A079326
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2)/2 - 1 for n even, a(n) = (n^2-n)/2 - 1 for n odd
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure we work in the correct integer type for large values. */
    seq_t nn = (seq_t)n;
    seq_t one = (seq_t)1;
    seq_t result;

    if ((n & (idx_t)1) == (idx_t)0)      /* even n */
    {
        result = (nn * nn) / (seq_t)2 - one;
    }
    else                                 /* odd n */
    {
        result = (nn * (nn - one)) / (seq_t)2 - one;
    }

    return result;
}
