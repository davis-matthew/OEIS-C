/*
 * A043360  Numbers having four 2's in base 5.
 * https://oeis.org/A043360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = numbers having four 2's in base 5
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t count = 0;          /* number of found values */
    seq_t    val    = 1;         /* current candidate */

    for (;;)
    {
        seq_t t    = val;
        unsigned twos = 0;

        /* count the number of 2's in the base‑5 representation */
        while (t > 0)
        {
            if ((t % 5) == 2)
            {
                if (++twos > 4)   /* early exit: more than four */
                    break;
            }
            t /= 5;
        }

        if (twos == 4)
        {
            ++count;
            if (count == n)
                return val;     /* found the nth term */
        }

        val = val + 1;          /* next integer */
    }
}
