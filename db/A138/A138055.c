/*
 * A138055  Period 8: repeat [1, 3, 5, 9, 3, 1, 9, 5].
 * https://oeis.org/A138055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) repeats every 8 terms: [1,3,5,9,3,1,9,5]
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[8] = { 1, 3, 5, 9, 3, 1, 9, 5 };
    return tbl[n & 7];
}
