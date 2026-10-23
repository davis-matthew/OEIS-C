/*
 * A043482  Numbers having two 7's in base 9.
 * https://oeis.org/A043482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer whose base‑9 representation
   contains exactly two occurrences of the digit 7. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;            /* number of found terms so far */

    for (seq_t val = 0; ; ++val)        /* iterate all integers */
    {
        seq_t tmp = val;
        int  c7   = 0;                /* count of 7's in base‑9 */

        /* count digits 7 in base 9 (at least one digit) */
        do
        {
            if ((tmp % 9) == 7) ++c7;
            tmp /= 9;
        } while (tmp > 0);

        if (c7 == 2)                 /* exactly two 7's */
        {
            ++count;
            if (count == n) return val;
        }
    }
}
