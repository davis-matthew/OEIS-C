/*
 * A017209  a(n) = 9*n + 4.
 * https://oeis.org/A017209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n + 4
 * Range:     u8 n <= 27, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nr = (seq_t)n;          /* convert index to seq_t */
    return nr * (seq_t)9 + (seq_t)4;
}
