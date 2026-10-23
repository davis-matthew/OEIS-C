/*
 * A174659  Numbers whose balanced ternary representation has more positive
 *          trits than negative trits.
 * https://oeis.org/A174659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 9486, u32 n <= 9486, u64 n <= 9486,
 *            u128 n <= 9486, big any n
 * Helpers:   bal_trit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = nth integer whose balanced ternary digit sum is > 0 */
/* (Numbers whose balanced ternary representation has more positive than negative trits) */

#include <stdbool.h>
#include "../utils/bal_trit_sum.h"

/* Returns the n-th term of the sequence (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* harmless for non‑positive indices */

    idx_t found = 0;
    seq_t val   = 0;

    for (seq_t cur = 1; ; ++cur) {
        if (bal_trit_sum(cur) > 0) {
            ++found;
            if (found == n) {
                val = cur;
                break;
            }
        }
    }
    return val;
}
