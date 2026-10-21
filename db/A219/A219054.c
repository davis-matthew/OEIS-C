/*
 * A219054  a(n) = (8*n^3 + 3*n^2 + n) / 6.
 * https://oeis.org/A219054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^3 + 3*n^2 + n) / 6
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 819, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* convert index to seq_t */
    seq_t n2 = nn * nn;             /* n² */
    seq_t n3 = n2 * nn;             /* n³ */
    seq_t numerator = (seq_t)8 * n3 + (seq_t)3 * n2 + nn;   /* 8n³ + 3n² + n */
    return numerator / (seq_t)6;    /* divide by 6 */
}
