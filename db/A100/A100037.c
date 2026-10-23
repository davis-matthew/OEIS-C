/*
 * A100037  Positions of occurrences of the natural numbers as a second
 *          subsequence in A100035.
 * https://oeis.org/A100037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^2 - n + 3
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert to seq_t before multiplying to avoid truncation. */
    seq_t tn = (seq_t)n;
    /* Compute 2*n^2 - n + 3 as tn * (2*tn - 1) + 3 */
    return (tn * (tn * 2 - 1)) + 3;
}
