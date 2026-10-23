/*
 * A043860  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 2 mod 9.
 * https://oeis.org/A043860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   * a(n) = nth integer with the number of runs in its base‑4
 *            representation * congruent to 2 modulo 9. (Offset = 1, a(1) = 4)
 * Range:     u8 n <= 54, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   has_run_condition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_run_condition.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val   = (seq_t)1;

    while (true) {
        if (has_run_condition(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}
