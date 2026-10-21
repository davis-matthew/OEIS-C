/*
 * A056020  Numbers that are congruent to +-1 mod 9.
 * https://oeis.org/A056020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*floor((n-1)/2) + (n odd ? 1 : 8)
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t half = (seq_t)(n - 1) / 2;
    seq_t base = 9 * half;
    return (n & 1) ? base + 1 : base + 8;
}
