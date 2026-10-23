/*
 * A011538  Numbers that contain an 8.
 * https://oeis.org/A011538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 3439, u32 n <= 3439, u64 n <= 3439,
 *            u128 n <= 3439, big any n
 * Helpers:   contains8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A011538 – Numbers that contain an 8
   a(n) = the n-th positive integer whose decimal representation includes the digit 8
   (OEIS offset: 1 – a(1) = 8) */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/contains8.h"

seq_t a(idx_t n)
{
    /* Ensure n >= 1 as per OEIS offset; behaviour for n == 0 is undefined. */
    seq_t curr = 0;
    idx_t  seen = 0;

    while (seen < n) {
        curr++;
        if (contains8(curr))
            seen++;
    }
    return curr;
}
