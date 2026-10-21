/*
 * A070431  a(n) = n^2 mod 6.
 * https://oeis.org/A070431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 6
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 6 first to avoid overflow in seq_t multiplication. */
    seq_t m = (seq_t)(n % 6);   /* n mod 6 */
    return (m * m) % (seq_t)6; /* (n mod 6)^2 mod 6 == n^2 mod 6 */
}
