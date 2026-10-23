/*
 * A135265  Period 6: repeat [1, 1, 1, 2, 2, 2].
 * https://oeis.org/A135265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 1 if n%6<3 else 2 (period 6, pattern [1,1,1,2,2,2])
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)(n % 6 < 3 ? 1 : 2);
}
