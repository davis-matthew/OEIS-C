/*
 * A064706  Square of permutation defined by A003188.
 * https://oeis.org/A064706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n XOR floor(n/4)
 * Range:     u8 n <= 255, u16 n <= 8530, u32 n <= 8530, u64 n <= 8530,
 *            u128 n <= 8530, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n ^ (seq_t)(n / 4);
}
