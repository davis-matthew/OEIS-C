/*
 * A044013  Numbers k such that 2 and 3 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has23or32_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series: A044013
   Computation: Numbers k whose decimal representation contains
   the adjacent digits 23 or 32, but the next integer k+1 does not.
   This implementation performs a linear scan over integers, checking
   the substring condition by inspecting digit pairs.
*/
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has23or32_u64.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (uint64_t k = 23; ; ++k)
    {
        if (has23or32_u64(k) && !has23or32_u64(k + 1))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
