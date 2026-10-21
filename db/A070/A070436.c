/*
 * A070436  a(n) = n^2 mod 13.
 * https://oeis.org/A070436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 13
 * Range:     u8 n <= 306, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = (seq_t)n % 13;
    return (r * r) % 13;
}
