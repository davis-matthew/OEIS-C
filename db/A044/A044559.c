/*
 * A044559  Numbers n such that string 5,5 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑7 representation
 *            contains the adjacent digit pair 5,5 but whose successor does
 *            not
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_55
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_55.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    idx_t count = 0;
    for (seq_t candidate = 1;; ++candidate)
    {
        if (has_55(candidate) && !has_55(candidate + 1))
        {
            ++count;
            if (count == n)
                return candidate;
        }
    }
}
