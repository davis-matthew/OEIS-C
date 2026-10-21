/*
 * A070486  a(n) = n^3 mod 24 (or equivalently, n^5 mod 24).
 * https://oeis.org/A070486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 24
 * Range:     u8 n <= 325, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = (seq_t)n % 24;          /* reduce n modulo 24 first */
    seq_t r2 = (r * r) % 24;          /* r^2 mod 24   */
    seq_t r3 = (r2 * r) % 24;          /* r^3 mod 24   */
    return r3;
}
