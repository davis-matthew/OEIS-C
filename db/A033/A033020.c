/*
 * A033020  Numbers whose base-7 expansion has no run of digits with length <
 *          2.
 * https://oeis.org/A033020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 297, u32 n <= 1200, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Helpers:   has_valid_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-variant: Enumerate positive integers, keep those whose base‑7
   representation has no run of a single digit.  This implements
   sequence A033020. */

#include <stdbool.h>   /* for bool */
#include "../utils/has_valid_run.h"

seq_t a(idx_t n)
{
    /* Return the n‑th term (1‑based) of the sequence. */
    idx_t count = 0;
    seq_t x = 1;

    while (1) {
        if (has_valid_run(x)) {
            count++;
            if (count == n)
                return x;
        }
        x++;
    }
}
