/*
 * A039188  Numbers whose base-11 representation has the same number of 3's
 *          and 10's.
 * https://oeis.org/A039188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   good_base11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039188: numbers whose base-11 representation has the same number of
 * 3's and 10's (digit value 10).  The function returns the nth such number
 * (n >= 1).  The computation is performed by a simple scan, checking each
 * non‑negative integer until the nth match is found. */

#include <stdbool.h>
#include "../utils/good_base11.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t val   = 0;

    while (found < n) {
        if (good_base11(val))
            ++found;
        if (found == n)
            return val;
        val++; /* increment for next candidate */
    }
    /* This point will never be reached because the loop exits when found==n. */
    return val;
}
