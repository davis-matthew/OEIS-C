/*
 * A070439  a(n) = n^2 mod 16.
 * https://oeis.org/A070439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 16
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 16 first to avoid overflow. */
    idx_t m = n & 15;                 /* n % 16 */
    seq_t r = (seq_t)m;               /* cast to seq_t */
    return (r * r) % (seq_t)16;       /* return (n^2 mod 16) */
}
