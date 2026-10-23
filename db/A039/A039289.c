/*
 * A039289  Numbers whose base-5 representation has the same nonzero number of
 *          1's and 4's.
 * https://oeis.org/A039289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nth number whose base‑5 representation has the same nonzero number of 1's and 4's.
   Search sequentially; returns the nth such number (offset 1). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1; safety */
    idx_t found = 0;
    unsigned __int128 cand = 1;                /* candidate number */

    while (1) {
        unsigned __int128 tmp = cand;
        uint8_t c1 = 0, c4 = 0;                /* counts of 1 and 4 */
        while (tmp > 0) {
            uint8_t d = (uint8_t)(tmp % 5);
            if (d == 1) ++c1;
            else if (d == 4) ++c4;
            tmp /= 5;
        }
        if (c1 > 0 && c1 == c4) {
            ++found;
            if (found == n) /* nth term found */
                return (seq_t)cand;
        }
        ++cand;                                 /* next integer */
    }
}
