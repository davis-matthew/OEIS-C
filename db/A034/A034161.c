/*
 * A034161  Number of partitions of n into 3 distinct parts from [ 1,18 ].
 * https://oeis.org/A034161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into 3 distinct parts from [1,18]; i <
 *            j < k, 1 ≤ i, j, k ≤ 18, and i + j + k = n
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 6 or n > 51 the sum cannot be formed with three distinct parts from 1..18 */
    if (n < 6 || n > 51)
        return 0;

    seq_t count = 0;
    for (idx_t i = 1; i <= 18; ++i)
    {
        for (idx_t j = i + 1; j <= 18; ++j)
        {
            for (idx_t k = j + 1; k <= 18; ++k)
            {
                seq_t sum = (seq_t)i + (seq_t)j + (seq_t)k;
                if (sum == n)
                    ++count;
            }
        }
    }
    return count;
}
