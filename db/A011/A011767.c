/*
 * A011767  From studying monochromatic solutions to x3-x2=x2-x1+2n.
 * https://oeis.org/A011767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 9, 14, 17, 21, 25 for n = 0..4; a(n) = 4*n + 10 for n >=
 *            5
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[5] = {9, 14, 17, 21, 25};
    if (n < 5)
        return tbl[n];
    return (seq_t)( (seq_t)n * 4UL + 10 );
}
