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
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A043118  
    Numbers k such that 0 and 3 occur juxtaposed in the base-6 representation
    of k but not of k-1.
    The function scans sequentially for the nth such number. */

#include <stdbool.h>

/* Return true if the base-6 representation of x contains the digits
   0 and 3 adjacent (in either order). */
static bool has_adjacent_0_3(seq_t x)
{
    if (x == 0)
        return false;

    seq_t prev = x % 6;     /* least significant digit */
    x /= 6;
    while (x > 0)
    {
        seq_t cur = x % 6;
        if ((prev == 0 && cur == 3) || (prev == 3 && cur == 0))
            return true;
        prev = cur;
        x /= 6;
    }
    return false;
}

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
