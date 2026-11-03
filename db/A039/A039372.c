/*
 * A039372  Numbers whose base-9 representation has the same nonzero number of
 *          2's and 3's.
 * https://oeis.org/A039372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose base-9 representation *
 *            contains an equal nonzero number of 2's and 3's
 * Range:     u8 n <= 20, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 8885 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool has_equal_nonzero_23(seq_t x)
{
    seq_t c2 = 0, c3 = 0;
    while (x) {
        int d = (int)(x % 9);
        if (d == 2) ++c2;
        else if (d == 3) ++c3;
        x /= 9;
    }
    return c2 > 0 && c2 == c3;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t found = 0;
    for (seq_t x = 1;; ++x) {
        if (has_equal_nonzero_23(x)) {
            ++found;
            if (found == n) return x;
        }
    }
}
