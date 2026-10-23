/*
 * A187288  Number of 3-step one or two space at a time rook's tours on an n X
 *          n board summed over all starting positions.
 * https://oeis.org/A187288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 56*n^2 - 160*n + 72 for n>3 Special cases: a(1)=0,
 *            a(2)=8, a(3)=108
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)0;
    if (n == 2u)
        return (seq_t)8;
    if (n == 3u)
        return (seq_t)108;

    seq_t nn = (seq_t)n;
    return (seq_t)(56 * nn * nn - 160 * nn + 72);
}
