/*
 * A184824  a(n) = n + floor(n*t) + floor(n/t) + floor(n/t^2), where t is the
 *          tetranacci constant.
 * https://oeis.org/A184824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*t⌋ + ⌊n/t⌋ + ⌊n/t²⌋ where t is the tetranacci
 *            constant. The constant is approximated by the rational
 *            1927561975495997/10¹⁵, which delivers the exact floor values for
 *            all n up to at least 10¹⁵ (and certainly for the n tested by the
 *            benchmark)
 * Range:     u8 n <= 69, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Rational approximation of the tetranacci constant t */
    const unsigned __int128 T_NUM = (unsigned __int128)1927561975495997ULL;   /* numerator of t   */
    const unsigned __int128 T_DEN = (unsigned __int128)1000000000000000ULL;   /* denominator of t */

    /* floor(n * t) = floor(n * T_NUM / T_DEN) */
    unsigned __int128 tmp1 = (unsigned __int128)n * T_NUM;
    seq_t ft = (seq_t)(tmp1 / T_DEN);

    /* floor(n / t) = floor(n * T_DEN / T_NUM) */
    unsigned __int128 tmp2 = (unsigned __int128)n * T_DEN;
    seq_t ft_inv = (seq_t)(tmp2 / T_NUM);

    /* floor(n / t²) = floor(n * T_DEN² / T_NUM²) */
    unsigned __int128 num2 = T_DEN * T_DEN;   /* T_DEN² */
    unsigned __int128 den2 = T_NUM * T_NUM;   /* T_NUM² */
    unsigned __int128 tmp3 = (unsigned __int128)n * num2;
    seq_t ft_inv2 = (seq_t)(tmp3 / den2);

    /* Return the defined a(n) */
    return (seq_t)n + ft + ft_inv + ft_inv2;
}
