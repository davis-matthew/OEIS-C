/*
 * A043904  Numbers k such that 1 and 5 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that the base‑6 *
 *            representation of k contains the digits 1 and 5 adjacent in
 *            either * order (15 or 51) and the base‑6 representation of k+1
 *            does not
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_adjacent_15_51
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_adjacent_15_51.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* sequence is defined for n≥1; undefined otherwise */
        return (seq_t)0;

    idx_t found = 0;
    seq_t k = 0;

    while (found < n) {
        if (has_adjacent_15_51(k) && !has_adjacent_15_51(k + (seq_t)1)) {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }

    /* Should never be reached */
    return (seq_t)0;
}
