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
 * Helpers:   zigzag_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032902: nth integer whose base‑5 digits satisfy
   d(0) <= d(1) >= d(2) <= d(3) >= …   (zig‑zag pattern) */
#include <stdbool.h>
#include "../utils/zigzag_base5.h"

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
