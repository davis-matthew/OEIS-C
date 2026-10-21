/*
 * A068073  Period 4 sequence [ 1, 2, 3, 2, ...].
 * https://oeis.org/A068073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = [1, 2, 3, 2][n mod 4] (periodic sequence of period 4)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[4] = {1, 2, 3, 2};
    return tbl[n & 3];
}
