/*
 * A163815  a(n) = n*(2*n^2 + 5*n + 3).
 * https://oeis.org/A163815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 + 5*n + 3)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = n; /* convert index to seq_t */
    return tn * (tn * tn * 2 + tn * 5 + 3);
}
