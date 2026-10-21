/*
 * A068491  Expansion of Molien series for a certain 4-D group of order 96.
 * https://oeis.org/A068491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3 + 3*n^2 + (18+3*(-1)^n)*n + 36)/36)
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;           /* convert index to seq_t */
    seq_t n2 = tn * tn;            /* n^2   */
    seq_t n3 = n2 * tn;            /* n^3   */
    /* coefficient: 21 if n even, 15 if n odd */
    seq_t coeff = ((n & (idx_t)1) == (idx_t)0) ? (seq_t)21 : (seq_t)15;
    /* numerator = n^3 + 3*n^2 + coeff*n + 36 */
    seq_t numerator = n3 + (seq_t)3 * n2 + coeff * tn + (seq_t)36;
    return numerator / (seq_t)36;
}
