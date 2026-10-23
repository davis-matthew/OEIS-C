/*
 * A132765  a(n) = n*(n + 23).
 * https://oeis.org/A132765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+23)
 * Range:     u8 n <= 8, u16 n <= 244, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast index to seq_t, then compute the quadratic formula */
    seq_t tn = (seq_t)n;
    return tn * (tn + 23);
}
