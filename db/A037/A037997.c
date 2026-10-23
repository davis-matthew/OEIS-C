/*
 * A037997  Numbers whose maximal base-9 run length is 2.
 * https://oeis.org/A037997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑9 representation has
 *            maximal run length exactly 2 (no digit repeats three or more
 *            times consecutively, but at least one run of two)
 * Range:     u8 n <= 42, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   has_max_run_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_max_run_2.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t i = 1; ; ++i) {
        if (has_max_run_2(i)) {
            count++;
            if (count == n) return i;
        }
    }
}
