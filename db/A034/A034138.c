/*
 * A034138  Number of partitions of n into distinct parts from [1, 8].
 * https://oeis.org/A034138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Subset sum enumeration for distinct parts 1-8
   a(n) = number of subsets of {1,2,…,8} whose elements sum to n */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cache of counts for all possible sums (0..36) */
    static seq_t cache[37];
    static bool initialized = false;

    if (!initialized)
    {
        /* initialise cache to zero */
        for (idx_t i = 0; i < 37; ++i)
            cache[i] = 0;

        /* enumerate all 2^8 subsets */
        for (idx_t mask = 0; mask < (1ULL << 8); ++mask)
        {
            idx_t sum = 0;
            for (int k = 0; k < 8; ++k)
                if (mask & (1ULL << k))
                    sum += (k + 1);
            /* sum is at most 36, cast to idx_t to index cache */
            cache[sum] += 1;
        }
        initialized = true;
    }

    /* n > 36 yields no valid partitions */
    if (n > 36)
        return 0;
    return cache[n];
}
