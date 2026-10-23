/*
 * A193766  The number of dominoes in a largest saturated domino covering of
 *          the 3 by n board.
 * https://oeis.org/A193766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - (3*n+4)/4 (integer division, floor)
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t three_n = 3 * nn;
    seq_t sub = (three_n + 4) / 4;  /* floor((3*n+4)/4) */
    return three_n - sub;
}
