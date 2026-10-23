/*
 * A034155  Number of partitions of n into 3 distinct parts from [ 1,12 ].
 * https://oeis.org/A034155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered triples (i, j, k) with * 1 ≤ i < j <
 *            k ≤ 12 and i + j + k = n. * This counts partitions of n into 3
 *            distinct parts from [1,12]
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The smallest possible sum is 1+2+3 = 6,
     * the largest is 10+11+12 = 33.  Outside this range the count is zero. */
    if (n < 6 || n > 33)
        return (seq_t)0;

    seq_t count = 0;
    for (idx_t i = 1; i <= 10; ++i)
    {
        for (idx_t j = i + 1; j <= 11; ++j)
        {
            idx_t k = n - i - j;
            if (k > j && k <= 12)
                ++count;
        }
    }
    return count;
}
