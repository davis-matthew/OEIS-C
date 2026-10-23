/*
 * A187287  Number of 2-step one or two space at a time rook's tours on an n X
 *          n board summed over all starting positions.
 * https://oeis.org/A187287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*n^2 - 12*n for n>1, with a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t tn = (seq_t)n;
    return 8 * tn * tn - 12 * tn;
}
