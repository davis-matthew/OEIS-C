/*
 * A043607  Numbers whose base-5 representation has exactly 7 runs.
 * https://oeis.org/A043607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   count_runs_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth integer whose base‑5 representation contains exactly 7 runs */
#include <stdbool.h>
#include "../utils/count_runs_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1; n=0 not used */
        return 0;

    seq_t x = (seq_t)15625;   /* 5^6 – first 7‑digit number in base 5 */
    idx_t count = 0;

    while (1) {
        if (count_runs_seq_t(x) == 7) {
            ++count;
            if (count == n)
                return x;
        }
        ++x;
    }
}
