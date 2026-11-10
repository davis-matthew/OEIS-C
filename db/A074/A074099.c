/*
 * A074099  Duplicate of A006788.
 * https://oeis.org/A074099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = floor(2^(n-1)/n), by binary long division of 2^(n-1) by n
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 21, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A006788_a(idx_t n)
{
    seq_t q = 0;
    uint64_t r = 0;
    idx_t i;
    for (i = 0; i < n; i++) {
        uint64_t bit = (i == 0) ? 1 : 0;
        r = 2 * r + bit;
        q = q * 2;
        if (r >= n) {
            r -= n;
            q = q + 1;
        }
    }
    return q;
}

seq_t a(idx_t n)
{
    return A006788_a(n);
}
