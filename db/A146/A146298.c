/*
 * A146298  Difference between the cubes and 2*tetrahedral numbers; A000578(n)
 *          - 2*A000292(n).
 * https://oeis.org/A146298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n-2)*n*(2*n+1)/3
 * Range:     i8 n <= 4, i16 n <= 25, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to signed seq_t once */
    seq_t ns = (seq_t)n;
    /* compute numerator components */
    seq_t term1 = ns - (seq_t)2;           /* n-2 */
    seq_t term2 = ns;                     /* n   */
    seq_t term3 = (seq_t)((seq_t)2 * ns + (seq_t)1); /* 2*n + 1 */
    /* multiply and divide */
    seq_t prod = term1 * term2;
    prod = prod * term3;
    return prod / (seq_t)3;
}
