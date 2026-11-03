/*
 * A039144  Numbers whose base-10 representation has the same number of 4's
 *          and 8's.
 * https://oeis.org/A039144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth nonnegative integer whose decimal representation
 *            contains the same number of 4's and 8's (including 0 which
 *            contains none)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool equal_4_8(seq_t v)
{
    seq_t t = v;
    /* 0 is counted: 0 4's and 0 8's  */
    if (t == 0) return true;

    int c4 = 0, c8 = 0;
    while (t != 0)
    {
        int d = (int)(t % 10);   /* digit */
        if (d == 4)
            c4++;
        else if (d == 8)
            c8++;
        t /= 10;
    }
    return c4 == c8;
}

seq_t a(idx_t n)
{
    seq_t num = 0;
    idx_t found = 0;   /* counts how many qualifying numbers we have seen */

    while (1)
    {
        if (equal_4_8(num))
        {
            found++;
            if (found == n)
                return num;
        }
        num++;   /* next integer to test */
    }
}
