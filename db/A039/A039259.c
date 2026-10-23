/*
 * A039259  Numbers whose base-12 representation has the same number of 5's
 *          and 11's.
 * https://oeis.org/A039259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   has_equal5_and11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: numbers whose base‑12 representation has equal count of 5's and 11's. */

#include <stdbool.h>
#include "../utils/has_equal5_and11.h"

seq_t a(idx_t n) {
    seq_t candidate = 0;
    idx_t found = 0;
    while (1) {
        if (has_equal5_and11(candidate)) {
            ++found;
            if (found == n) return candidate;
        }
        ++candidate;
    }
}
