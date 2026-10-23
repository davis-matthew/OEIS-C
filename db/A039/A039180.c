/*
 * A039180  Numbers whose base-11 representation has the same number of 2's
 *          and 9's.
 * https://oeis.org/A039180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base-11 representation has the same number of 2's and 9's */
/* a(1) = 0 is the first term (base-11 "0") */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Count terms until we reach the requested index */
    idx_t count = 0;
    seq_t val = 0;
    while (count < n) {
        /* Check if val is valid: equal count of digits 2 and 9 in base‑11 */
        seq_t x = val;
        unsigned c2 = 0, c9 = 0;
        if (x == 0) {
            /* 0 has zero 2's and zero 9's – valid */
            c2 = 0;
            c9 = 0;
        } else {
            while (x > 0) {
                unsigned digit = (unsigned)(x % 11);
                if (digit == 2) ++c2;
                else if (digit == 9) ++c9;
                x /= 11;
            }
        }
        if (c2 == c9) {
            ++count;
            if (count == n) {
                return val;
            }
        }
        ++val;
    }
    /* Should never be reached for valid inputs */
    return 0;
}
