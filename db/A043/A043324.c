/*
 * A043324  Numbers having four 0's in base 3.
 * https://oeis.org/A043324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043324: Numbers having four 0's in base 3
   Implementation: iterate through positive integers, count zeros
   in their base‑3 representation, and return the n‑th number that
   has exactly four zeros. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1; value for n=0 is undefined */
        return 0;

    idx_t found = 0;
    seq_t val   = 0;          /* start from 0, the loop will look at 1… */

    for (;;) {
        /* Count zeros in base‑3 representation of val */
        seq_t t  = val;
        uint8_t zeros = 0;

        if (t == 0) {
            /* representation "0" has one zero; this value will not
               contribute to the sequence, so we can skip incrementing
               found here. */
            zeros = 1;
        } else {
            while (t > 0) {
                if ((t % 3) == 0)
                    ++zeros;
                t /= 3;
            }
        }

        if (zeros == 4) {
            ++found;
            if (found == n)
                return val;
        }

        ++val;     /* next integer to test */
    }
}
