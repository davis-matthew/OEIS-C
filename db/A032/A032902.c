/*
 * A032902  Numbers whose base-5 representation Sum_{i=0..m} d(i)*5^i has d(0)
 *          <= d(1) >= d(2) <= ...
 * https://oeis.org/A032902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032902: nth integer whose base‑5 digits satisfy
   d(0) <= d(1) >= d(2) <= d(3) >= …   (zig‑zag pattern) */
#include <stdbool.h>

static bool zigzag_base5(seq_t x)
{
    /* x is assumed to be > 0 */
    seq_t prev = x % 5;
    x /= 5;
    bool le = true; /* first comparison is d0 <= d1 */

    while (x > 0) {
        seq_t curr = x % 5;

        if (le) {
            if (prev > curr) return false;   /* needs prev <= curr */
        } else {
            if (prev < curr) return false;   /* needs prev >= curr */
        }

        prev = curr;
        le = !le;
        x /= 5;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* n is 1‑based */
    idx_t seen = 0;
    seq_t val = 1;

    while (1) {
        if (zigzag_base5(val)) {
            ++seen;
            if (seen == n)
                return val;
        }
        ++val;
    }
}
