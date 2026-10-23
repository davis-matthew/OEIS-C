/*
 * A010695  Period 2: repeat (2,5).
 * https://oeis.org/A010695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n % 2 == 0 ? 2 : 5) (period 2: 2,5)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n & (idx_t)1) ? (seq_t)5 : (seq_t)2;
}
