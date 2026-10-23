/*
 * A151883  Let g be a permutation of [1..n] having say j_i cycles of length
 *          i, with Sum_i i*j_i = n; sequence gives Sum_g Sum_{i even}
 *          (j_i)^2.
 * https://oeis.org/A151883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * ( sum_{k=1}^{⌊n/2⌋} 1/(2k) + sum_{k=1}^{⌊n/4⌋}
 *            1/(4k^2) ) (equivalent to a(n) = Σ_{k=1}^{⌊n/2⌋} n!/(2k) +
 *            Σ_{k=1}^{⌊n/4⌋} n!/(4k^2) )
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t fact = factorial(n);
    seq_t sum = 0;

    idx_t max1 = n / 2;
    for (idx_t k = 1; k <= max1; ++k)
        sum += fact / (seq_t)(2 * k);

    idx_t max2 = n / 4;
    for (idx_t k = 1; k <= max2; ++k)
        sum += fact / (seq_t)(4 * k * k);

    return sum;
}
