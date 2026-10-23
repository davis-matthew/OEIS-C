/*
 * A043653  Numbers whose base-12 representation has exactly 4 runs.
 * https://oeis.org/A043653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   has_four_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* numbers whose base-12 representation has exactly 4 runs */
#include <stdbool.h>
#include "../utils/has_four_runs.h"

/* main sequence function */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t x = 1;                  /* start from 1 (0 has 1 run) */
    while (true) {
        if (has_four_runs(x)) {
            found++;
            if (found == n)
                return x;
        }
        x++;
    }
}
