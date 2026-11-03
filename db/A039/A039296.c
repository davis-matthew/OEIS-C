/*
 * A039296  Numbers whose base-6 representation has the same nonzero number of
 *          0's and 4's.
 * https://oeis.org/A039296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search of integers that have the same positive
   number of 0's and 4's in their base‑6 representation. */
#include <stdbool.h>

static bool has_equal_nonzero_0_and_4(seq_t val)
{
    seq_t y = val;
    unsigned zeros = 0, fours = 0;
    const seq_t six = (seq_t)6;
    while (y > (seq_t)0) {
        unsigned d = (unsigned)(y % six);
        if (d == 0) {
            ++zeros;
        } else if (d == 4) {
            ++fours;
        }
        y /= six;
    }
    return zeros > 0 && zeros == fours;
}

seq_t a(idx_t n)
{
    idx_t      count = 0;
    seq_t      x     = (seq_t)1;

    while (1) {
        if (has_equal_nonzero_0_and_4(x)) {
            ++count;
            if (count == n) {
                return x;
            }
        }
        x += (seq_t)1;
    }
}
