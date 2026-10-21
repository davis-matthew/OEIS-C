/*
 * A091066  Numbers having in binary representation at least one proper prefix
 *          that is also a suffix.
 * https://oeis.org/A091066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_bordered_binary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091066: Numbers whose binary representation has at least one
   proper prefix that is also a suffix.
   Computed by incremental search from 3 upwards and testing each
   candidate for the existence of a border in its binary string. */

#include <stdbool.h>
#include "../utils/has_bordered_binary.h"

seq_t a(idx_t n)
{
    /* Return n-th element of A091066 (1‑based). */
    idx_t count = 0;
    for (idx_t cand = 3; ; ++cand) {
        if (has_bordered_binary((seq_t)cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}
