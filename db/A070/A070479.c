/*
 * A070479  a(n) = n^3 mod 17.
 * https://oeis.org/A070479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 17
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 17 to avoid large intermediate products. */
    seq_t r = (seq_t)(n % 17);
    /* r is in [0,16], so r^3 fits comfortably in seq_t. */
    return (seq_t)(r * r * r % 17);
}
