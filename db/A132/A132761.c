/*
 * A132761  a(n) = n*(n+17).
 * https://oeis.org/A132761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+17)
 * Range:     u8 n <= 9, u16 n <= 247, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;          /* convert index to the sequence type */
    return k * (k + (seq_t)17);   /* compute n*(n+17) using seq_t arithmetic */
}
