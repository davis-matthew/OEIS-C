/*
 * A070437  a(n) = n^2 mod 14.
 * https://oeis.org/A070437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 14
 * Range:     u8 n <= 265, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t, do the arithmetic in seq_t, and reduce mod 14. */
    seq_t n_seq = (seq_t)n;
    seq_t mod  = (seq_t)14;
    seq_t r = n_seq % mod;           /* r = n mod 14 */
    return (r * r) % mod;            /* a(n) = r^2 mod 14 */
}
