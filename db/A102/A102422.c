/*
 * A102422  Number of partitions of n with k <= 5 parts and each part p <= 5.
 * https://oeis.org/A102422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   number of partitions of n with at most 5 parts, each part ≤5
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   precompute
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/precompute.h"

seq_t a(idx_t n)
{
    static seq_t memo[26];
    static bool done = false;

    if (!done)
    {
        precompute(memo);
        done = true;
    }

    if (n > 25)
        return (seq_t)0;
    return memo[(int)n];
}
