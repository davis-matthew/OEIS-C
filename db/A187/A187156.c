/*
 * A187156  Number of 3-step one space at a time bishop's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*n^2 - 40*n + 32 for n ≥ 2, a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;
    seq_t nn = (seq_t)n;
    return 12 * nn * nn - 40 * nn + 32;
}
