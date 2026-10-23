/*
 * A043948  Numbers k such that 2 and 6 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer k such that the base‑8
 *            representation of k contains an adjacent pair of digits 2 and 6
 *            (in either order) while the base‑8 representation of k+1 does
 *            not. The sequence is obtained by a simple search
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adj_26
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_adj_26.h"

seq_t a(idx_t n)
{
    /* Find the n‑th number satisfying the condition. */
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        if (has_adj_26((seq_t)k) && !has_adj_26((seq_t)(k + 1))) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
