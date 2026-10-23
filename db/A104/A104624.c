/*
 * A104624  Expansion of exp( arcsinh( -2*x ) ) in powers of x.
 * https://oeis.org/A104624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   coefficients of sqrt(1+4*x^2)-2*x. a(0)=1, a(1)=-2,
 *            a(2k)=2*(-1)^{k-1}*C(2k-2,k-1)/k, a(odd>1)=0
 * Range:     i8 n <= 11, i16 n <= 19, i32 n <= 31, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)1;
    if (n == 1ULL)
        return (seq_t)-2;
    if (n & 1ULL)          /* odd index >1 */
        return (seq_t)0;

    idx_t k = n >> 1;      /* n = 2*k */
    /* binomial (2k-2 choose k-1) */
    seq_t bin = (seq_t)1;
    for (idx_t i = 1; i < k; ++i) {
        seq_t num = (seq_t)(k - 1 + i);   /* (k-1)+i = i + k - 1 */
        bin = bin * num / (seq_t)i;
    }
    seq_t val = bin * (seq_t)2 / (seq_t)k;
    if ((k - 1) & 1ULL)   /* sign = (-1)^{k-1} : odd -> negative */
        return -val;
    else
        return val;
}
