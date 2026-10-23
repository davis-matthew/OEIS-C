/*
 * A039266  Numbers whose base-12 representation has the same number of 7's
 *          and 9's.
 * https://oeis.org/A039266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 183, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_equal_7_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-12 representation has the same number of 7's and 9's. */
#include <stdbool.h>
#include "../utils/has_equal_7_9.h"

/* a(n) = the n-th non‑negative integer whose base‑12 representation has equal
 * numbers of 7’s and 9’s.  a(1) = 0. */
seq_t a(idx_t n)
{
    seq_t val = 0;
    idx_t count = 0;
    while (1)
    {
        if (has_equal_7_9(val))
        {
            ++count;
            if (count == n)
                return val;
        }
        ++val;   /* wrap‑around is fine for the ranges used in the tests */
    }
}
