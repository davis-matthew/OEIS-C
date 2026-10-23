/*
 * A043993  Numbers k such that 6 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adj_6_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation scans integers in increasing order and returns the
 * nth integer k such that the base-9 representation of k contains two
 * adjacent digits 6 and 7 in either order, while the base-9 representation
 * of k+1 does not. */

#include <stdbool.h>
#include "../utils/has_adj_6_7.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safe default for out of range */

    idx_t found = 0;

    for (uint64_t k = 0;; ++k) {
        if (has_adj_6_7(k) && !has_adj_6_7(k + 1)) {
            ++found;
            if (found == n) return (seq_t)k;
        }
    }
}
