/*
 * A175054  A positive integer n is included if there is no run of 0's in the
 *          binary representation of n that is only one digit long.
 * https://oeis.org/A175054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth positive integer whose binary representation
 *            contains no isolated zero run (no consecutive 0's of length
 *            exactly 1)
 * Range:     u8 n <= 85, u16 n <= 7738, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_isolated_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_isolated_zero.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; undefined for n==0 */

    seq_t candidate = 1;
    idx_t count    = 0;

    for (;; ++candidate)
    {
        if (!has_isolated_zero(candidate))
        {
            if (++count == n)
                return candidate;
        }
    }
}
