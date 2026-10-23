/*
 * A048067  Number of nonempty subsets of {1,2,...,n} in which exactly 3/5 of
 *          the elements are <= (n-4)/2.
 * https://oeis.org/A048067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{t >= 1} binomial(floor((n-4)/2), 3t) * binomial(n -
 *            floor((n-4)/2), 2t)
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where the sum runs over t such that 3t <= floor((n-4)/2) and 2t <= n - floor((n-4)/2). */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 5)
        return (seq_t)0;                     /* values for n = 1..4 are 0 */

    idx_t m = (n - 4) / 2;                    /* floor((n-4)/2) */
    idx_t t_max1 = m / 3;                     /* from 3t <= m   */
    idx_t t_max2 = (n - m) / 2;               /* from 2t <= n-m */
    idx_t t_max = t_max1 < t_max2 ? t_max1 : t_max2;

    seq_t sum = (seq_t)0;
    for (idx_t t = 1; t <= t_max; ++t)
    {
        seq_t c1 = binomial(m, 3 * t);
        seq_t c2 = binomial(n - m, 2 * t);
        sum += c1 * c2;
    }
    return sum;
}
