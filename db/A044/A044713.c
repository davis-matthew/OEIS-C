/*
 * A044713  Numbers n such that string 0,0 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th number whose decimal representation contains
   the substring "00" while the next integer n+1 does not contain it. */
#include <stdbool.h>

static bool has00(seq_t x)
{
    seq_t prev = 10;                 /* sentinel > 9 */
    while (x != 0)
    {
        seq_t d = x % 10;
        if (prev == 0 && d == 0)
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for offset 1 */

    seq_t val   = 1;
    idx_t  count = 0;

    /* iterate until the n-th qualifying number is found */
    while (true)
    {
        if (has00(val))
        {
            seq_t next = val + 1;
            if (!has00(next))
            {
                ++count;
                if (count == n)
                    return val;
            }
        }
        ++val;
    }
}
