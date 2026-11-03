/*
 * A039134  Numbers whose base-10 representation has the same number of 2's
 *          and 9's.
 * https://oeis.org/A039134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039134: numbers whose decimal representation has the same
 * number of 2's and 9's. This implementation searches sequentially
 * from 0 upward until the nth such number is found. */
#include <stdbool.h>

/* Count and return the nth (1‑based) number satisfying the condition. */
static bool equal_2_9(seq_t x)
{
    int c2 = 0, c9 = 0;
    if (x == 0)
        return true;                 /* 0 has no 2's and no 9's */
    while (x > 0)
    {
        int d = (int)(x % 10);
        if (d == 2) ++c2;
        else if (d == 9) ++c9;
        x = x / 10;
    }
    return c2 == c9;
}

seq_t a(idx_t n)
{
    seq_t cand = 0;
    idx_t count = 0;
    while (count < n)
    {
        if (equal_2_9(cand))
        {
            ++count;
            if (count == n)
                return cand;
        }
        cand = cand + 1;
    }
    /* Should never reach here for n >= 1 */
    return 0;
}
