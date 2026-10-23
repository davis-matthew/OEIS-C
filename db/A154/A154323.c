/*
 * A154323  Central coefficients of number triangle A113582.
 * https://oeis.org/A154323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + ((n*(n+1)/2)^2) (equivalently (n^4+2*n^3+n^2+4)/4)
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 361, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;               /* index converted to seq_t */
    seq_t t = sn * (sn + (seq_t)1);    /* n*(n+1) */
    t = t / (seq_t)2;                  /* divide by 2 safely */
    seq_t res = t * t;                 /* t^2 */
    res = res + (seq_t)1;              /* +1 */
    return res;
}
