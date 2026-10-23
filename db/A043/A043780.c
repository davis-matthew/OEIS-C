/*
 * A043780  Number of runs in the base 3 representation of n is congruent to 0
 *          mod 5.
 * https://oeis.org/A043780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_runs_div5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that the number of runs in the base‑3 representation of n is 0 mod 5.
   a(n) returns the nth such number (offset 1). */
#include <stdbool.h>
#include "../utils/has_runs_div5.h"

seq_t a(idx_t n)
{
    seq_t cand = 1;         /* candidate number to test */
    idx_t found = 0;        /* how many qualifying numbers seen so far */

    while (1) {
        if (has_runs_div5(cand)) {
            found++;
            if (found == n)
                return cand;
        }
        cand++;              /* next integer to check */
    }
}
