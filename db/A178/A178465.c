/*
 * A178465  Expansion of -2*x^2*(-3-2*x+x^2-x^3-2*x^4+x^5) / ( (1+x)^2*(x-1)^4
 *          ).
 * https://oeis.org/A178465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=0. For n even: a(n) = n*(n^2+2)/2 For n odd>1: a(n)
 *            = (n+1)*(n^2-n+2)/2
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    seq_t sn = (seq_t)n;
    if ((n & 1) == 0U) {            /* n is even */
        seq_t tmp = sn * sn + (seq_t)2;
        return sn * tmp / (seq_t)2;
    } else {                        /* n is odd (n > 1) */
        seq_t tmp = sn * sn - sn + (seq_t)2;
        return (sn + (seq_t)1) * tmp / (seq_t)2;
    }
}
