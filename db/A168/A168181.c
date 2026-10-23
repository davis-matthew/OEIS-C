/*
 * A168181  Characteristic function of numbers that are not multiples of 8.
 * https://oeis.org/A168181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = 1 if n mod 8 != 0 else 0
 * Range:     u8 n <= 9925, u16 n <= 9925, u32 n <= 9925, u64 n <= 9925,
 *            u128 n <= 9925, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n % 8) ? (seq_t)1 : (seq_t)0;
}
