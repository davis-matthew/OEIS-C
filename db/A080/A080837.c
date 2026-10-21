/*
 * A080837  Jouka numbers: either the number is divisible by 7 or 11 or has a
 *          7 in its decimal representation.
 * https://oeis.org/A080837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = nth positive integer such that the number is divisible by
 *            7, divisible by 11, or contains a digit 7
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_seven
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_seven.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n==0, but return 0 */

    idx_t count = 0;
    idx_t candidate = 1;

    while (true)
    {
        if ((candidate % 7 == 0) || (candidate % 11 == 0) || has_seven((seq_t)candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
