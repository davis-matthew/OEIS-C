/*
 * A044385  Numbers n such that string 5,3 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   _contains53
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: a(n) = the nth positive integer whose decimal representation
 * contains the substring "53" but whose predecessor does not. */

#include <stdbool.h>
#include "../utils/_contains53.h"

seq_t a(idx_t n)
{
    if (n == 0)        /* offset is 1; behaviour for 0 is irrelevant */
        return (seq_t)0;

    idx_t count = 0;
    idx_t current = 0;

    while (count < n)
    {
        ++current;
        if (_contains53(current))
        {
            if (!_contains53(current - 1))
                ++count;
        }
    }

    return (seq_t)current;
}
