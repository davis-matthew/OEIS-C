/*
 * A070446  a(n) = n^2 mod 24.
 * https://oeis.org/A070446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 24
 * Range:     u8 n <= 264, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nm = (seq_t)n % 24;
    return (nm * nm) % 24;
}
