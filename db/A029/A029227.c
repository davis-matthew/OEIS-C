/*
 * A029227  Expansion of 1/((1-x^2)*(1-x^7)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A029227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+51*n^2+776*n+6720)/6720 - (n^2+27*n+156)*(n
 *            mod 2)/320)
 * Range:     u8 n <= 0, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the sequence type */
    seq_t N = (seq_t)n;

    /* numerator of the first rational term, denominator 6720 */
    seq_t num = N * N * N          /* n^3 */
             + (seq_t)51 * N * N   /* 51*n^2 */
             + (seq_t)776 * N      /* 776*n  */
             + (seq_t)6720;        /* +6720  */

    /* if n is odd, subtract the second term */
    if ((n & 1) != 0) {
        seq_t second = (N * N          /* n^2  */
                      + (seq_t)27 * N   /* 27*n */
                      + (seq_t)156)     /* +156 */
                      * (seq_t)21;      /* 21 = 6720/320 */
        num -= second;
    }

    /* floor division by 6720 */
    return num / (seq_t)6720;
}
