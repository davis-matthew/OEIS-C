/*
 * A039532  Numbers whose base-12 representation has the same nonzero number
 *          of 4's and 6's.
 * https://oeis.org/A039532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool has_equal_nonzero_4_and_6(seq_t v)
{
    seq_t x = v;
    int c4 = 0, c6 = 0;
    while (x > 0) {
        int d = (int)(x % 12);
        if (d == 4) ++c4;
        else if (d == 6) ++c6;
        x /= 12;
    }
    return c4 > 0 && c4 == c6;
}

/* Sequence: numbers whose base-12 representation has an equal positive count of
   digits 4 and 6, indexed with a(1) = 54. */

seq_t a(idx_t n)
{
    seq_t v = 0;
    idx_t found = 0;

    while (1) {
        if (has_equal_nonzero_4_and_6(v)) {
            ++found;
            if (found == n) return v;
        }
        ++v;                /* next candidate */
    }
}
