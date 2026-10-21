/*
 * A081492  Sum of terms in n-th row of A081491.
 * https://oeis.org/A081492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^3 - 6*n^2 + 13*n - 3)/6
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 220, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1 but safe guard */

    seq_t nn = n;                /* convert index to seq_t */
    seq_t term1 = (seq_t)2 * nn * nn * nn;   /* 2*n^3 */
    seq_t term2 = (seq_t)6 * nn * nn;        /* 6*n^2 */
    seq_t term3 = (seq_t)13 * nn;            /* 13*n */

    seq_t numerator = nn * (term1 - term2 + term3 - (seq_t)3);
    return numerator / (seq_t)6;             /* exact division */
}
