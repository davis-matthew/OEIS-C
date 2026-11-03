/*
 * A044720  Numbers n such that string 0,7 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   nth integer whose decimal representation contains the substring
 *            "07" but n+1 does not
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool contains07(seq_t x)
{
    /* Return true if the base‑10 representation of x contains the consecutive digits 0 followed by 7. */
    if (x < 10) return false;      /* single digit cannot contain "07" */
    seq_t prev = x % 10;
    x /= 10;
    while (x > 0)
    {
        seq_t cur = x % 10;
        if (cur == 0 && prev == 7) return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, but guard for safety */

    seq_t count  = 0;
    seq_t current = 0;     /* candidate numbers start from 1 */

    while (count < n)
    {
        current++; /* next candidate */
        if (contains07(current))
        {
            seq_t next = current + 1;
            if (!contains07(next))
            {
                if (++count == n)
                    return current;
            }
        }
    }

    /* Should never reach here */
    return 0;
}
