/*
 * A010877  a(n) = n mod 8.
 * https://oeis.org/A010877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n mod 8
 * Range:     u8 n <= 9917, u16 n <= 9917, u32 n <= 9917, u64 n <= 9917,
 *            u128 n <= 9917, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)(n % 8);
}
