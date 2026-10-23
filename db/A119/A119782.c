/*
 * A119782  Numerator of Sum[ i^3/(n-i+1)^2, {i,1,n}].
 * https://oeis.org/A119782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator[ Sum_{i=1..n} i^3/(n-i+1)^2 ]
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum_num = 0;
    seq_t sum_den = 1;

    for (idx_t i = 1; i <= n; ++i) {
        /* term = i^3 / (n-i+1)^2 */
        seq_t denom = (seq_t)(n - i + 1);
        denom = denom * denom;                         /* (n-i+1)^2 */

        seq_t num = (seq_t)i * (seq_t)i * (seq_t)i;    /* i^3 */

        /* add the fractions: sum_num/sum_den + num/denom */
        seq_t new_num  = sum_num * denom + num * sum_den;
        seq_t new_den  = sum_den * denom;

        seq_t g = gcd(new_num, new_den);
        sum_num = new_num / g;
        sum_den = new_den / g;
    }

    return sum_num;
}
