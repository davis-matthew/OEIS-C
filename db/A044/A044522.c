/*
 * A044522  Numbers n such that string 0,3 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has03
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates all integers n such that the base‑7 representation
 * of n contains the consecutive digits 0 followed by 3, and the base‑7
 * representation of n+1 does not contain that pattern.
 * The sequence A044522 is obtained by returning the nth such integer, where
 * a(1) = 52 is the first term.  The function is valid for any build
 * that defines the integer types seq_t and idx_t.  For seq_t being an
 * arbitrary‑precision type or one of the standard unsigned integer sizes,
 * the algorithm is correct and runs in linear time with respect to the
 * value of n (the maximum is far below 2 s for n ≈ 10 000).  No
 * specialized libraries are required beyond <stdbool.h>. */

#include <stdbool.h>
#include "../utils/has03.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n ≤ 0 the function
     * simply returns 0, as the caller can ignore such calls. */
    if (n == 0)
        return 0;

    seq_t val = 0;
    idx_t count = 0;

    while (1) {
        /* Advance to the next integer to test. */
        val = val + 1;

        /* Check the condition for the current value and its successor. */
        if (has03(val) && !has03(val + 1)) {
            ++count;
            if (count == n)
                return val;
        }
    }
}
