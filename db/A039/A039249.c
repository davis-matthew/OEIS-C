/*
 * A039249  Numbers whose base-12 representation has the same number of 4's
 *          and 7's.
 * https://oeis.org/A039249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-12 representation has the same number of 4's
 *            and 7's
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_equal_4_7(idx_t x) {
    unsigned c4 = 0, c7 = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 12);
        if (d == 4) ++c4;
        else if (d == 7) ++c7;
        x /= 12;
    }
    return c4 == c7;
}

seq_t a(idx_t n) {
    idx_t val   = 0;
    idx_t seen  = 0;
    while (1) {
        if (has_equal_4_7(val)) {
            ++seen;
            if (seen == n)
                return (seq_t)val;
        }
        ++val;
    }
}
