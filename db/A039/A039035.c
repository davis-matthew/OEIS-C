/*
 * A039035  Numbers whose base-7 representation has the same number of 1's and
 *          2's.
 * https://oeis.org/A039035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑7 representation has the same number of 1's and 2's
   (OEIS A039035).  The n‑th term is found by scanning the integers
   beginning at 0 and counting those whose base‑7 digits contain an
   equal number of 1's and 2's.  This runs in O(n * log₇(n)) time,
   which easily meets the 2‑second requirement for n ≤ 10 000. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n≥1 for valid calls */

    seq_t x = 0;                   /* candidate integer */
    idx_t found = 0;               /* how many qualifying numbers found */

    while (1)
    {
        int c1 = 0, c2 = 0;
        seq_t tmp = x;

        /* Base‑7 digit inspection */
        while (tmp > 0)
        {
            int r = (int)(tmp % 7);
            if (r == 1) ++c1;
            else if (r == 2) ++c2;
            tmp /= 7;
        }

        if (c1 == c2)            /* counts equal → qualifies */
        {
            ++found;
            if (found == n) return x;
        }
        ++x;                     /* advance to next integer */
    }
}
