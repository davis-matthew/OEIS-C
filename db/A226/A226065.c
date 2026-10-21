/*
 * A226065  Sum of all the smaller parts raised to their corresponding larger
 *          parts of the partitions of n into exactly two parts.
 * https://oeis.org/A226065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} i^(n-i)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 28, u128 n <= 47,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sum = 0;
    idx_t lim = n / 2;
    for (idx_t i = 1; i <= lim; ++i)
        sum += seq_pow((seq_t)i, n - i);

    return sum;
}
