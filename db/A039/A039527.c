/*
 * A039527  Numbers whose base-12 representation has the same nonzero number
 *          of 3's and 8's.
 * https://oeis.org/A039527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039527
   Numbers whose base‑12 representation has the same nonzero number of 3's
   and 8's.  The code searches sequentially for the n‑th such number,
   with an offset of 1 (a(1) = 44). */

#include <stdbool.h>   // for bool

seq_t a(idx_t n)
{
    /* offset 1 */
    if (n < 1) return 0;

    seq_t cur = 0;      /* current number being tested */
    idx_t found = 0;    /* how many valid numbers seen */

    /* search indefinitely until the n‑th number is found */
    for (;; ++cur)
    {
        seq_t tmp = cur;
        int c3 = 0, c8 = 0;

        /* count digits 3 and 8 in base‑12 representation */
        while (tmp > 0)
        {
            int d = (int)(tmp % 12);   /* digit in base 12 */
            if (d == 3) ++c3;
            else if (d == 8) ++c8;
            tmp /= 12;
        }

        /* accept only if both counts are equal and non‑zero */
        if (c3 > 0 && c3 == c8)
        {
            ++found;
            if (found == n) return cur;
        }
    }
}
