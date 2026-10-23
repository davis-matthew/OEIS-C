/*
 * A039457  Numbers whose base-11 representation has the same nonzero number
 *          of 2's and 3's.
 * https://oeis.org/A039457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers whose base-11 representation has the same nonzero
   number of digit 2's and digit 3's (OEIS A039457). This implementation
   iterates over integers in increasing order, testing the condition
   until the nth number is found. The algorithm is O(m) where m is the
   value of the nth term, which is well below the 2‑second limit for n
   up to 10000. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Not used, but keeps the function safe. */

    idx_t  found = 0;   /* How many valid numbers we have seen so far. */
    seq_t  x = 0;       /* Current candidate number. */

    while (found < n)
    {
        ++x;                               /* Next integer. */
        seq_t tmp = x;
        uint8_t c2 = 0, c3 = 0;

        /* Count the digits 2 and 3 in the base‑11 representation. */
        while (tmp > 0)
        {
            uint8_t d = (uint8_t)(tmp % 11);
            if (d == 2) ++c2;
            else if (d == 3) ++c3;
            tmp /= 11;
        }

        if (c2 == c3 && c2 > 0) ++found;
    }

    return x;
}
