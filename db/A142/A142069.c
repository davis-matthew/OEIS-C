/*
 * A142069  Period 9: repeat [3, 7, 2, 6, 1, 5, 0, 4, 8].
 * https://oeis.org/A142069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-9) (period 9: 3,7,2,6,1,5,0,4,8)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t cycle[9] = {3, 7, 2, 6, 1, 5, 0, 4, 8};
    return cycle[n % 9];
}
