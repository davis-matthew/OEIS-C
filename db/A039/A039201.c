/*
 * A039201  Numbers whose base-11 representation has the same number of 6's
 *          and 8's.
 * https://oeis.org/A039201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-11 representation has equal counts of digit 6 and digit 8. */
#include <stdbool.h>

static inline bool equal6and8(seq_t x) {
    seq_t six   = (seq_t)6;
    seq_t eight = (seq_t)8;
    seq_t cnt6 = 0;
    seq_t cnt8 = 0;
    while (x > 0) {
        seq_t d = x % (seq_t)11;
        if (d == six)
            ++cnt6;
        else if (d == eight)
            ++cnt8;
        x /= (seq_t)11;
    }
    return cnt6 == cnt8;
}

seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t x = 0;
    while (true) {
        if (equal6and8(x)) {
            ++found;
            if (found == n)
                return x;
        }
        ++x;
    }
}
