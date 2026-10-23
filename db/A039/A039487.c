/*
 * A039487  Numbers whose base-11 representation has the same nonzero number
 *          of 7's and 8's.
 * https://oeis.org/A039487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has_equal_7_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose base-11 representation has the same nonzero
 * number of 7's and 8's.
 */

#include <stdbool.h>
#include "../utils/has_equal_7_8.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t cur   = 1;
    while (count < n) {
        if (has_equal_7_8((seq_t)cur)) {
            count++;
            if (count == n)
                return (seq_t)cur;
        }
        cur++;
    }
    /* Unreachable for valid n>=1 */
    return (seq_t)0;
}
