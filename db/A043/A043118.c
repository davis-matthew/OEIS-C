/*
 * A043118  Numbers k such that 0 and 3 occur juxtaposed in the base-6
 *          representation of k but not of k-1.
 * https://oeis.org/A043118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_adjacent_0_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A043118  
    Numbers k such that 0 and 3 occur juxtaposed in the base-6 representation
    of k but not of k-1.
    The function scans sequentially for the nth such number. */

#include <stdbool.h>
#include "../utils/has_adjacent_0_3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* not used – offset is 1 */

    idx_t count = 0;
    for (seq_t k = 0;; ++k)
    {
        if (k != 0 && has_adjacent_0_3(k))
        {
            if (!has_adjacent_0_3(k - 1))
            {
                ++count;
                if (count == n)
                    return k;
            }
        }
    }
}
