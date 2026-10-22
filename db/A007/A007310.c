/*
 * A007310  Numbers congruent to 1 or 5 mod 6.
 * https://oeis.org/A007310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 1 - (n mod 2)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s_n = (seq_t)n;
    return 3 * s_n - 1 - (s_n & 1);
}
