/*
 * A055052  Numbers of the form 4^i*(8j+7) or 4^i*(8j+5).
 * https://oeis.org/A055052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_desired
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Predicate: integer m is of the form 4^i*(8j+5) or 4^i*(8j+7). */

#include <stdbool.h>
#include "../utils/is_desired.h"

seq_t a(idx_t n)
{
    /* Offsets are 1-based: a(1) = 5. */
    seq_t cur = 1;
    idx_t count = 0;
    while (count < n) {
        if (is_desired(cur))
            ++count;
        ++cur;
    }
    return cur - 1;
}
