/*
 * A039450  Numbers whose base-11 representation has the same nonzero number
 *          of 1's and 4's.
 * https://oeis.org/A039450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nᵗʰ positive integer whose base‑11 representation
 *            contains the same nonzero number of 1’s and 4’s (both >0)
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   has_equal_ones_fours
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_equal_ones_fours.h"

seq_t a(idx_t n)
{
    seq_t value = (seq_t)1;
    idx_t found = 0;
    while (found < n)
    {
        if (has_equal_ones_fours(value))
        {
            ++found;
            if (found == n)
                return value;
        }
        ++value;       /* seq_t increment is defined for all supported types */
    }
    return (seq_t)0;   /* unreachable */
}
