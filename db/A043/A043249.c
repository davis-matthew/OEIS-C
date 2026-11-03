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
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043249: Numbers k such that 4 and 8 occur juxtaposed in the
   base‑10 representation of k but not of k-1. Computed by scanning
   integers and testing the adjacency condition. */
#include <stdbool.h>

static bool contains_adj48or84(seq_t x)
{
    seq_t prev = 10;  /* sentinel for no previous digit */
    while (x > 0)
    {
        seq_t d = x % 10;
        if (prev != 10)
        {
            if ((prev == 4 && d == 8) || (prev == 8 && d == 4))
                return true;
        }
        prev = d;
        x = x / 10;
    }
    return false;
}

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
