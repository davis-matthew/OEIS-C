/*
 * A043253  Numbers k such that 5 and 8 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer k such that the decimal digits 5 and *
 *            8 appear adjacent (as 58 or 85) in k, but not in k − 1
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent_58_or_85
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_adjacent_58_or_85.h"

/* return the n-th term of the sequence; n ≥ 1 */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t k = (seq_t)58;          /* the first possible candidate */
    while (true) {
        if (has_adjacent_58_or_85(k) &&
            !has_adjacent_58_or_85(k - (seq_t)1)) {
            found++;
            if (found == n)
                return k;
        }
        ++k;                       /* next integer */
    }
}
