/*
 * A173985  a(n) = numerator of (Zeta(0,2,2/3) - Zeta(0,2,n+2/3)), where Zeta
 *          is the Hurwitz Zeta function.
 * https://oeis.org/A173985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of Sum_{k=0..n-1} 9/(3*k+2)^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined by the rational sum
       a(n) = numerator of Σ_{k=0}^{n-1} 9/(3k+2)^2
       The fraction is reduced after each addition.       */
    if (n == 0)
        return 0;

    seq_t num = 0;      /* Current numerator   */
    seq_t den = 1;      /* Current denominator */

    for (idx_t k = 0; k < n; ++k)
    {
        /* Denominator of the k‑th term: (3k+2)^2 */
        uint64_t base = 3 * (uint64_t)k + 2;
        seq_t d = (seq_t)base * (seq_t)base;

        /* Reduce the product before multiplying */
        seq_t g1 = gcd(den, d);
        seq_t den_div = den / g1;      /* den/g1      */
        seq_t d_div   = d   / g1;      /* d/g1        */

        /* New numerator: num * d_div + 9 * den_div */
        seq_t part1   = num * d_div;
        seq_t part2   = (seq_t)9 * den_div;
        seq_t new_num = part1 + part2;

        /* LCM of den and d is den_div * d  */
        seq_t new_den = den_div * d;

        /* Reduce the resulting fraction */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return num;
}
