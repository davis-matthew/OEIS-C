/*
 * A043926  Numbers k such that 3 and 4 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers k whose base–7 representation contains the
   adjacent digits 3 and 4 (either 34 or 43) but k + 1 does not. */
#include <stdbool.h>

static bool has_adj_34_43(idx_t v)
{
    if (v < 7) return false;                /* at least two base‑7 digits are needed */
    idx_t low = v % 7;
    v /= 7;
    while (v > 0)
    {
        idx_t high = v % 7;
        if ((low == 3 && high == 4) || (low == 4 && high == 3))
            return true;                   /* found adjacent 3·4 or 4·3 */
        low = high;
        v /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    for (;; ++k)
    {
        if (has_adj_34_43(k))
        {
            idx_t k1 = k + 1;
            if (!has_adj_34_43(k1))
            {
                ++count;
                if (count == n)
                    return (seq_t)k;
            }
        }
    }
}
