/*
 * A039205  Numbers whose base-11 representation has the same number of 7's
 *          and 9's.
 * https://oeis.org/A039205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes the n-th integer whose base‑11 representation has an equal
   number of 7’s and 9’s.  A simple linear scan is sufficient for
   all values of n that appear in the test data. */
static bool equal_7_9(seq_t x)
{
    seq_t c7 = 0, c9 = 0;
    while (x > 0) {
        seq_t d = x % 11;
        x     /= 11;
        if (d == 7) c7++;
        else if (d == 9) c9++;
    }
    return c7 == c9;
}

seq_t a(idx_t n)
{
    seq_t cand  = 0;
    idx_t count = 0;

    while (true) {
        if (equal_7_9(cand)) {
            count++;
            if (count == n)
                return cand;
        }
        cand++;
    }
}
