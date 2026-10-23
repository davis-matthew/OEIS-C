/*
 * A023706  Numbers with a single 0 in their base 4 expansion.
 * https://oeis.org/A023706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A023706: numbers with exactly one 0 in their base‑4 expansion */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; return the n‑th term */
    idx_t  cnt = 0;
    seq_t  x   = 0;

    for (;; ++x) {
        idx_t zeros = 0;

        if (x == (seq_t)0) {          /* special case for 0 */
            zeros = 1;
        } else {
            seq_t y = x;
            while (y > (seq_t)0) {    /* examine each base‑4 digit */
                if ((y % (seq_t)4) == (seq_t)0)
                    ++zeros;
                y /= (seq_t)4;
                if (zeros > 1)        /* early exit if more than one zero */
                    break;
            }
        }

        if (zeros == 1) {             /* term found */
            ++cnt;
            if (cnt == n)
                return x;
        }
    }
}
