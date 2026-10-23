/*
 * A032883  Numbers whose base-4 representation Sum_{i=0..m} d(i)*4^i has d(0)
 *          < d(1) > d(2) < ...
 * https://oeis.org/A032883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   alt_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032883:
   Numbers whose base-4 representation satisfies
   d0 < d1 > d2 < d3 > …  (alternating up/down, starting with <
   where d0 is the least‑significant digit). */

#include <stdbool.h>
#include "../utils/alt_base4.h"

/* Main function as required by the build system */
seq_t a(idx_t n)
{
    /* n is 1‑based. */
    idx_t count = 0;
    seq_t val = 1;

    while (true) {
        if (alt_base4(val)) {
            ++count;
            if (count == n) return val;
        }
        ++val;
    }
}
