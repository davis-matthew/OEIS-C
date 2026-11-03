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
 * Validated: standard, pass, 79 terms through n = 9709 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_isolated_zero(seq_t x)
{
    seq_t y = x;
    seq_t zero_run = 0;
    while (y != 0)
    {
        if ((y & 1) == 0)
            zero_run++;
        else
        {
            if (zero_run == 1)          /* isolated 0 detected */
                return true;
            zero_run = 0;
        }
        y >>= 1;
    }
    return zero_run == 1;               /* trailing zero run of length 1 */
}

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
