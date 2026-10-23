/*
 * A131870  Period 8: repeat [1, 2, 3, 4, 6, 7, 8, 9].
 * https://oeis.org/A131870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = repeat [1,2,3,4,6,7,8,9] with period 8
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t vals[8] = { 1, 2, 3, 4, 6, 7, 8, 9 };
    return vals[n & 7];
}
