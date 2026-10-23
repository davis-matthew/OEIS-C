/*
 * A023699  Numbers with a single 2 in their ternary expansion.
 * https://oeis.org/A023699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose base‑3 expansion contains
 *            exactly one digit 2
 * Range:     u8 n <= 85, u16 n <= 6144, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_single_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/has_single_two.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t count = 0;
    seq_t val = 1;
    while (1)
    {
        if (has_single_two(val))
        {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}
