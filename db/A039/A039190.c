/*
 * A039190  Numbers whose base-11 representation has the same number of 4's
 *          and 6's.
 * https://oeis.org/A039190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth nonnegative integer whose base‑11 representation
 *            * has the same number of 4's and 6's. The sequence is indexed
 *            starting at * 1 (a(1) = 0)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool __eq46(seq_t x)
{
    seq_t t = x;
    int c4 = 0;
    int c6 = 0;
    if (t == 0)
        return true;          /* base‑11 "0" has no 4 or 6 */
    while (t != 0)
    {
        int d = (int)(t % (seq_t)11);
        if (d == 4)
            ++c4;
        else if (d == 6)
            ++c6;
        t /= (seq_t)11;
    }
    return c4 == c6;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* below offset, any value is acceptable */

    idx_t found = 0;
    seq_t candidate = 0;

    while (true)
    {
        if (__eq46(candidate))
        {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
}
