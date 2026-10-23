/*
 * A043416  Numbers whose base-7 representation contains exactly four 5's.
 * https://oeis.org/A043416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑7 representation
 *            contains exactly four 5's. (OEIS A043416)
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   has_four_fives_in_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>    /* for standard types (if needed) */
#include "../utils/has_four_fives_in_base7.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* We search sequentially for the nth number with the property. */
    seq_t candidate = (seq_t)0;
    idx_t found = (idx_t)0;

    while (found < n) {
        ++candidate;                     /* move to next integer */
        if (has_four_fives_in_base7(candidate))
            ++found;                     /* successful candidate */
    }

    return candidate;
}
