/*
 * A047196  Number of nonempty subsets of {1,2,...,n} in which exactly 1/4 of
 *          the elements are <= n/3.
 * https://oeis.org/A047196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1<=j<=n/4} binomial(floor(n/3), j) * binomial(n -
 *            floor(n/3), 3*j)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 36, u64 n <= 69, u128 n <= 135,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t n3 = n / 3;
    idx_t n4 = n / 4;
    seq_t sum = 0;
    for (idx_t j = 1; j <= n4; ++j)
    {
        seq_t term = binomial(n3, j);
        term = term * binomial(n - n3, 3 * j);
        sum += term;
    }
    return sum;
}
