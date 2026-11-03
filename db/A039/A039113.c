/*
 * A039113  Numbers whose base-10 representation has the same number of 0's
 *          and 2's.
 * https://oeis.org/A039113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   compute the nth integer whose decimal representation contains
 *            the same number of 0's and 2's. Brute‑force scan
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_equal_zero_and_two(seq_t x) {
    if (x == 0) {
        return false;           /* representation "0" has 1 zero, 0 twos */
    }
    int zeros = 0, twos = 0;
    while (x > 0) {
        int d = (int)(x % 10);
        if (d == 0) {
            ++zeros;
        } else if (d == 2) {
            ++twos;
        }
        x /= 10;
    }
    return zeros == twos;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t val   = 1;
    while (count < n) {
        if (has_equal_zero_and_two(val)) {
            ++count;
            if (count == n) {
                return val;
            }
        }
        ++val;
    }
    return 0; /* Should never happen for valid n */
}
