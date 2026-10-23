/*
 * A010691  Period 2: repeat (1,10).
 * https://oeis.org/A010691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, word
 * Formula:   a(n) = 10^n mod 11
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n & 1) ? (seq_t)10 : (seq_t)1;
}
