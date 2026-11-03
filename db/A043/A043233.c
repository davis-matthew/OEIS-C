/*
 * A043233  Numbers k such that 2 and 3 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer k such that k’s decimal
 *            representation contains the substring "23" or "32", but (k-1)’s
 *            representation does not
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Check if x contains the adjacent digits 23 or 32. */
static bool has23or32(seq_t x)
{
    seq_t prev = 10;               /* sentinel value */
    while (x > 0)
    {
        seq_t last = x % 10;
        if (prev != 10 &&
            ((prev == 2 && last == 3) || (prev == 3 && last == 2)))
            return true;
        prev = last;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for offset < 1, return 0 */

    seq_t k = 1;                   /* current integer to test */
    idx_t count = 0;               /* how many qualifying numbers found */

    for (;; ++k)
    {
        if (has23or32(k))
        {
            if (!has23or32(k - 1))
            {
                ++count;
                if (count == n)
                    return k;
            }
        }
    }
}
