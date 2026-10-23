/*
 * A121782  Series expansion for mean-squared radius of gyration of rectangles
 *          on square lattice.
 * https://oeis.org/A121782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*(n-1)*(n^2+2)/3
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn     = (seq_t)n;        /* convert index to seq_t */
    seq_t n2     = sn * sn;         /* n^2 */
    seq_t nminus1 = sn - (seq_t)1;  /* n-1 */
    seq_t n2plus2 = n2 + (seq_t)2;  /* n^2+2 */

    seq_t prod = n2 * nminus1;      /* n^2*(n-1) */
    prod = prod * n2plus2;          /* * (n^2+2) */
    return prod / (seq_t)3;         /* divide by 3 */
}
