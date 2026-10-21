/*
 * A091756  Seventh column (m=8) of array A091752 ((-1,2)Stirling2).
 * https://oeis.org/A091756
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3)*(9*n^3-45*n^2+36*n+40)*(3*(n-3))! / *
 *            (80*3^(n-3)*(n-3)!) for n ≥ 4 (offset 4)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   reduce_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reduce_factor.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;               /* offset is 4 */

    idx_t k = n - 3;                   /* k = n-3, k ≥ 1 */
    seq_t sn = (seq_t)n;

    /* Polynomial part: 9*n^3 - 45*n^2 + 36*n + 40 */
    seq_t poly = (seq_t)9 * sn * sn * sn
               - (seq_t)45 * sn * sn
               + (seq_t)36 * sn
               + (seq_t)40;

    seq_t result = 1;
    idx_t exp3 = k;      /* factors of 3 from the denominator 3^(n-3) */
    idx_t exp2 = 4;      /* 2^4 part of the denominator 80 */
    idx_t exp5 = 1;      /* 5 part of the denominator 80 */

    /* incorporate the k factor */
    reduce_factor(&result, &exp3, &exp2, &exp5, (seq_t)k);

    /* incorporate the polynomial factor */
    reduce_factor(&result, &exp3, &exp2, &exp5, poly);

    /* multiply the remaining factors of the ratio
       (3k)! / k! = Π_{i=k+1}^{3k} i
    */
    for (idx_t i = k + 1; i <= 3 * k; ++i)
        reduce_factor(&result, &exp3, &exp2, &exp5, (seq_t)i);

    /* any remaining denominator factors (should be none) */
    while (exp3 > 0) { result *= 3; exp3--; }
    while (exp2 > 0) { result *= 2; exp2--; }
    while (exp5 > 0) { result *= 5; exp5--; }

    return result;
}
