/*
 * A190718  Quadruplicated tetrahedral numbers A000292.
 * https://oeis.org/A190718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(floor(n/4)+3,3)
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor division by 4 */
    idx_t k = n >> 2;               /* k = floor(n/4) */
    seq_t kk = (seq_t)k;           /* convert to seq_t */

    seq_t t1 = kk + 3;
    seq_t t2 = kk + 2;
    seq_t t3 = kk + 1;

    seq_t num = t1 * t2;
    num *= t3;

    seq_t res = num / 6;           /* exact division */
    return res;
}
