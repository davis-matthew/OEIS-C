/*
 * A043249  Numbers k such that 4 and 8 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   contains_adj48or84
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043249: Numbers k such that 4 and 8 occur juxtaposed in the
   base‑10 representation of k but not of k-1. Computed by scanning
   integers and testing the adjacency condition. */
#include <stdbool.h>
#include "../utils/contains_adj48or84.h"

seq_t a(idx_t n)
{
    seq_t cur = 1;
    idx_t cnt = 0;
    while (cnt < n)
    {
        if (contains_adj48or84(cur))
        {
            if (!contains_adj48or84(cur - 1))
            {
                ++cnt;
                if (cnt == n)
                    return cur;
            }
        }
        ++cur;
    }
    return cur;
}
