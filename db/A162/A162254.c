/*
 * A162254  a(n) = n*(2*n^2 + 5*n + 1)/2.
 * https://oeis.org/A162254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 + 5*n + 1)/2
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;              /* convert index to seq_t */
    seq_t n2 = sn * sn;              /* n^2 */
    seq_t expr = (seq_t)2 * n2 + (seq_t)5 * sn + (seq_t)1;  /* 2*n^2 + 5*n + 1 */
    seq_t prod = sn * expr;          /* n * expr */
    return prod / 2;                 /* divide by 2 (exact) */
}
