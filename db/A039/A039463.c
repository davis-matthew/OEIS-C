/*
 * A039463  Numbers whose base-11 representation has the same nonzero number
 *          of 2's and 9's.
 * https://oeis.org/A039463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth positive integer whose base‑11 representation
 *            contains an equal nonzero number of digits 2 and 9
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_equal_2_and_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/has_equal_2_and_9.h"

/* Returns the nth term (1‑based) of the sequence.  The routine scans
   integers in increasing order and counts those that satisfy the
   required property until the desired index is reached.  This
   implementation requires O(n) time and O(1) auxiliary memory, well
   within the 2‑second limit for n up to about 10 000. */
seq_t a(idx_t n)
{
    seq_t candidate = (seq_t)1;
    idx_t  found    = (idx_t)0;

    while (found < n) {
        if (has_equal_2_and_9(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }

    /* Should never be reached. */
    return (seq_t)0;
}
