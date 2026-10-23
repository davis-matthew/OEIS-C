/*
 * A037364  Numbers whose base-6 and base-8 expansions have no digits in
 *          common.
 * https://oeis.org/A037364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   disjoint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the nth positive integer whose base‑6 and base‑8
 * expansions have no common digit. */
#include <stdbool.h>
#include "../utils/disjoint.h"

seq_t a(idx_t n)
{
    idx_t   count = 0;
    seq_t   x = 0;

    while (count < n) {
        ++x;
        if (disjoint(x))
            ++count;
    }
    return x;
}
