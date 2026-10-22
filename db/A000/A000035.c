/*
 * A000035  Period 2: repeat [0, 1]; a(n) = n mod 2; parity of n.
 * https://oeis.org/A000035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, core, easy, nonn, nice, mult
 * Formula:   a(n) = n mod 2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)(n & 1u);
}
