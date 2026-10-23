/*
 * A039453  Numbers whose base-11 representation has the same nonzero number
 *          of 1's and 7's.
 * https://oeis.org/A039453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for numbers whose base‑11 representation has an equal
   nonzero number of 1’s and 7’s.  The function returns the n‑th such
   number (1‑indexed). */

#include <stdint.h>

static int good_base11(seq_t x)
{
    seq_t t = x;
    unsigned long long c1 = 0, c7 = 0;
    while (t)
    {
        seq_t d = t % (seq_t)11;
        if (d == (seq_t)1)
            ++c1;
        else if (d == (seq_t)7)
            ++c7;
        t /= (seq_t)11;
    }
    return (c1 > 0 && c1 == c7);
}

seq_t a(idx_t n)
{
    seq_t num = 1;
    idx_t found = 0;
    while (1)
    {
        if (good_base11(num))
        {
            ++found;
            if (found == n)
                return num;
        }
        ++num;
    }
}
