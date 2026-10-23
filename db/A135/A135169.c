/*
 * A135169  Period 4: repeat [1, 5, 9, 5].
 * https://oeis.org/A135169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 - 4*cos(n*Pi/2) = period 4: [1, 5, 9, 5]
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* periodic with period 4, indexed from 0 */
    static const seq_t tbl[4] = {1, 5, 9, 5};
    return tbl[n & 3ULL];
}
