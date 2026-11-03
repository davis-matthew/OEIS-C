/*
 * A039339  Numbers whose base-8 representation has the same nonzero number of
 *          1's and 5's.
 * https://oeis.org/A039339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based generation of numbers whose base‑8 representation
   has equal nonzero numbers of 1's and 5's (OEIS A039339). */

#include <stdbool.h>

static bool has_equal_ones_and_fives_base8(seq_t value)
{
    int c1 = 0, c5 = 0;
    seq_t v = value;
    while (v > 0) {
        int digit = (int)(v & (seq_t)7);      /* v % 8, using bitwise AND */
        if (digit == 1) ++c1;
        else if (digit == 5) ++c5;
        v >>= 3;                               /* v /= 8, using right shift */
    }
    return (c1 > 0) && (c1 == c5);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe guard */

    idx_t found = 0;
    seq_t num = 1;                 /* start from 1 */

    for (;; ++num) {
        if (has_equal_ones_and_fives_base8(num)) {
            ++found;
            if (found == n)
                return num;
        }
    }
}
