/*
 * A145625  Numerator of the polynomial A_l(x) = sum_{d=1..l-1} x^(l-d)/d for
 *          index l=2n+1 evaluated at x=9.
 * https://oeis.org/A145625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) is the numerator of the rational number S_{2n+1}(9) where
 *            S_l(x) = sum_{k=1}^{l-1} x^k/(l-k). Recurrence: S_{l+1} = 9 *
 *            S_l + 9/l (l ≥ 2), with S_2 = 9. This yields the desired
 *            numerator after reduction at each step
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Starting value S_2 = 9/1 */
    seq_t num = (seq_t)9;   /* numerator */
    seq_t den = (seq_t)1;   /* denominator */

    /* Iterate l = 2 … 2n to reach S_{2n+1} */
    for (idx_t l = 2; l <= 2 * n; ++l)
    {
        seq_t lseq = (seq_t)l;

        /* temp = l * num + den      (both are seq_t) */
        seq_t temp = num * lseq + den;

        /* new numerator = 9 * temp   */
        seq_t new_num = temp * (seq_t)9;

        /* new denominator = den * l   */
        seq_t new_den = den * lseq;

        /* Reduce fraction */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* After the loop, num is the required numerator */
    return num;
}
