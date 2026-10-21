/*
 * A068021  Z(S_m; sigma[1](n), sigma[2](n),..., sigma[m](n)) where Z(S_m;
 *          x_1,x_2,...,x_m) is the cycle index of the symmetric group S_m and
 *          sigma[k](n) is the sum of k-th powers of divisors of n; m=4.
 * https://oeis.org/A068021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/24*(sigma1(n)^4 + 6*sigma1(n)^2*sigma2(n) +
 *            8*sigma1(n)*sigma3(n) + 3*sigma2(n)^2 + 6*sigma4(n))
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    /* Compute the divisor sums of the required powers */
    seq_t s1 = sigma_k(n, 1);
    seq_t s2 = sigma_k(n, 2);
    seq_t s3 = sigma_k(n, 3);
    seq_t s4 = sigma_k(n, 4);

    /* Accumulate each term of the numerator */
    seq_t term1 = s1;
    term1 *= s1;            /* s1^2 */
    term1 *= s1;            /* s1^3 */
    term1 *= s1;            /* s1^4 */

    seq_t term2 = s1;
    term2 *= s1;            /* s1^2 */
    term2 *= s2;            /* s1^2*s2 */
    term2 *= 6;             /* 6*s1^2*s2 */

    seq_t term3 = s1;
    term3 *= s3;            /* s1*s3 */
    term3 *= 8;             /* 8*s1*s3 */

    seq_t term4 = s2;
    term4 *= s2;            /* s2^2 */
    term4 *= 3;             /* 3*s2^2 */

    seq_t term5 = s4;
    term5 *= 6;             /* 6*s4 */

    seq_t num = term1;
    num += term2;
    num += term3;
    num += term4;
    num += term5;

    /* Divide by 24 (which is 4! = 24) */
    return num / 24;
}
