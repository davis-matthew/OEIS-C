/*
 * A061877  |First digit - second digit + third digit - fourth digit ...| = 8.
 * https://oeis.org/A061877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest integer m such that |first digit – second digit
 *            + third digit – …| = 8
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   matches
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/matches.h"

seq_t a(idx_t n)
{
    seq_t cur = 1;          /* current integer being tested */
    idx_t count = 0;        /* number of terms found so far */

    while (count < n) {
        if (matches(cur)) {
            ++count;
        }
        cur = cur + 1;
    }

    /* cur has been incremented after the nth match, so subtract 1. */
    return cur - 1;
}
