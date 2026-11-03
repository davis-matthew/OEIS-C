/*
 * A039248  Numbers whose base-12 representation has the same number of 4's
 *          and 6's.
 * https://oeis.org/A039248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   A039248 – numbers whose base‑12 representation has the same
 *            number of 4's and 6's. Compute by a linear scan over integers
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether a number has an equal count of digits 4 and 6 in base 12. */
static bool has_equal_4_6(seq_t x)
{
    unsigned count4 = 0, count6 = 0;
    seq_t y = x;
    while (y != 0) {
        seq_t digit = y % 12;
        if (digit == 4) count4++;
        else if (digit == 6) count6++;
        y /= 12;
    }
    return count4 == count6;
}

seq_t a(idx_t n)
{
    seq_t candidate = 0;
    idx_t found = 0;
    while (found < n) {
        if (has_equal_4_6(candidate))
            ++found;
        ++candidate;
    }
    return candidate - 1;   /* last accepted number */
}
