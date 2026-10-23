/*
 * A039247  Numbers whose base-12 representation has the same number of 4's
 *          and 5's.
 * https://oeis.org/A039247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth integer (starting at n = 1) whose base‑12
 *            representation contains an equal number of digits 4 and 5. (a(1)
 *            = 0, a(2) = 1, …)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   has_equal_45
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_equal_45.h"

/* Return the nth term (n ≥ 1).  Works in any call order – it recomputes
   from scratch each time, which is fast enough for n up to about 10 000. */
seq_t a(idx_t n)
{
    seq_t cand = 0;
    idx_t found = 0;

    while (found < n) {
        if (has_equal_45(cand)) {
            ++found;
            if (found == n)
                return cand;
        }
        ++cand;
    }
    return cand; /* unreachable */
}
