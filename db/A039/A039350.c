/*
 * A039350  Numbers whose base-8 representation has the same nonzero number of
 *          3's and 7's.
 * https://oeis.org/A039350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-8 representation has a positive and equal
 *            number of digits 3 and 7. This function searches sequentially
 *            for such numbers
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* no term for index 0, defensive guard */

    seq_t cur   = 1;                /* current number to test          */
    idx_t found = 0;                /* how many valid numbers have
                                       been found so far               */

    while (true)
    {
        /* count 3's and 7's in base‑8 representation of cur */
        seq_t x      = cur;
        int   cnt3   = 0;
        int   cnt7   = 0;

        do
        {
            int d = (int)(x % 8);   /* remainder is in 0..7           */
            if (d == 3) ++cnt3;
            else if (d == 7) ++cnt7;
            x /= 8;                 /* integer division                */
        } while (x);                /* stop when all digits processed  */

        /* valid if both counts are positive and equal */
        if (cnt3 > 0 && cnt3 == cnt7)
        {
            ++found;
            if (found == n) return cur;
        }

        ++cur;                     /* test next number                */
    }
}
