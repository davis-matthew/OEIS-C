/*
 * A039364  Numbers whose base-9 representation has the same nonzero number of
 *          0's and 8's.
 * https://oeis.org/A039364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_nonzero_0_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS A039364.
   The sequence consists of the positive integers whose base‑9
   representation contains the same non‑zero number of 0's and 8's.
   The algorithm simply searches forward from 1 until it has found n
   such numbers.  No external libraries are required. */

#include <stdbool.h>
#include "../utils/has_equal_nonzero_0_8.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    seq_t cand = (seq_t)1;
    idx_t found = 0;

    while (1) {
        if (has_equal_nonzero_0_8(cand)) {
            ++found;
            if (found == n) return cand;
        }
        ++cand;
    }
}
