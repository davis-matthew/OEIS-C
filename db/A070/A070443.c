/*
 * A070443  a(n) = n^2 mod 21.
 * https://oeis.org/A070443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 21
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 21 before squaring to keep intermediate values small. */
    idx_t r = n % 21;
    return (seq_t)((r * r) % 21);
}
