/*
 * A137040  Numbers k such that k and k^2 use only the digits 1, 3, 6 and 8.
 * https://oeis.org/A137040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137040: numbers k such that k and k^2 use only the digits 1, 3, 6 and 8.
   The function enumerates numbers composed of these digits in increasing order and
   counts those whose square also uses only these digits.  The n‑th such number
   (n ≥ 1) is returned.  The algorithm runs in O(k·L) time where L is the
   length in decimal digits of k, which is fast enough for n up to about 10 000
   in all supported builds. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* check that every decimal digit of x is one of 1,3,6,8 */
static bool allowed_digits(seq_t x)
{
    if (x == 0) return false;
    while (x > 0) {
        seq_t d = x % 10;
        if (d != 1 && d != 3 && d != 6 && d != 8) return false;
        x = x / 10;
    }
    return true;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 0 */

    static const uint8_t digits[4] = {1, 3, 6, 8};

    idx_t count = 0;   /* how many valid numbers have been found */
    idx_t len   = 1;   /* current decimal length */

    while (true) /* iterate over increasing lengths */
    {
        /* dynamic array of base‑4 indices, most‑significant first */
        int *idx = (int *)malloc((size_t)len * sizeof(int));
        if (!idx) return 0;      /* allocation failure -> abort */

        for (idx_t i = 0; i < len; ++i) idx[i] = 0; /* initialise to all 1’s */

        while (true)   /* enumerate all numbers of this length */
        {
            /* Build the candidate number from the current base‑4 index. */
            seq_t val = 0;
            for (idx_t j = 0; j < len; ++j)
                val = val * 10 + digits[idx[j]];

            /* Check the digit constraint for val and for val^2. */
            if (allowed_digits(val) && allowed_digits(val * val))
            {
                ++count;
                if (count == n)
                {
                    free(idx);
                    return val;
                }
            }

            /* Increment the base‑4 counter (least‑significant digit last). */
            idx_t pos = len - 1; /* start from least‑significant */
            while (true)
            {
                idx[pos]++;                 /* increment current digit */
                if (idx[pos] < 4)           /* no carry */
                    break;
                idx[pos] = 0;               /* reset and carry to next */
                if (pos == 0)              /* overflow: done with this length */
                {
                    free(idx);
                    len++;                 /* go to next length */
                    goto next_length;
                }
                pos--;                      /* move to more significant digit */
            }
        }
next_length: ;
    }
}
