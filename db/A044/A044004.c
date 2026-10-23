/*
 * A044004  Numbers k such that 0 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer k such that the decimal *
 *            representation of k contains "09" or "90" as a pair of adjacent
 *            * digits, yet the representation of k+1 does not contain such a
 *            pair
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent_09_90
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_adjacent_09_90.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t k = 1;
    while (1) {
        if (has_adjacent_09_90(k) && !has_adjacent_09_90(k + 1)) {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }
}
