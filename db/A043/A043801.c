/*
 * A043801  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 3 mod 8.
 * https://oeis.org/A043801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   runs_is_3_mod_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043801: nth integer whose base‑3 representation has a number of runs ≡ 3 (mod 8). */

#include <stddef.h>
#include "../utils/runs_is_3_mod_8.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* a(0) is not defined; sequence is 1‑based */
    idx_t counted = 0;
    seq_t candidate = 1;
    while (1) {
        if (runs_is_3_mod_8(candidate)) {
            counted++;
            if (counted == n) return candidate;
        }
        candidate = candidate + 1;
    }
}
