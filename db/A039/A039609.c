/*
 * A039609  Numbers whose base-12 representation has the same nonzero number
 *          of 1's and 7's.
 * https://oeis.org/A039609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑12 representation has an equal, non‑zero
   number of digit 1’s and digit 7’s.  The sequence is indexed starting
   at n = 1. */
#include <stdbool.h>

static int qualify(seq_t x)
{
    seq_t t = x;
    int cnt1 = 0, cnt7 = 0;
    while (t > 0)
    {
        seq_t d = t % 12;
        if (d == 1)
            ++cnt1;
        else if (d == 7)
            ++cnt7;
        t = t / 12;
    }
    return (cnt1 > 0 && cnt7 > 0 && cnt1 == cnt7);
}

seq_t a(idx_t n)
{
    seq_t cand = 1;
    idx_t found = 0;
    while (found < n)
    {
        if (qualify(cand))
        {
            ++found;
            if (found == n)
                return cand;
        }
        ++cand;
    }
    return 0;  /* unreachable, but keeps the compiler happy */
}
