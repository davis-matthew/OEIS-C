/*
 * A140081  Period 4: repeat [0, 1, 1, 2].
 * https://oeis.org/A140081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n mod 4)+(n mod 2))/2 (period 4: [0,1,1,2])
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (((seq_t)(n & 3u)) + ((seq_t)(n & 1u))) / 2;
}
