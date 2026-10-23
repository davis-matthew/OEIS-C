/*
 * A101082  Numbers n such that binary representation contains bit strings
 *          "10" and "01" (possibly overlapping).
 * https://oeis.org/A101082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 219, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has10and01
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the nth integer whose binary representation contains
 * both substrings "10" and "01" (possibly overlapping).
 * The sequence starts with 5, 9, 10, 11, 13, ... as per OEIS A101082. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has10and01.h"

/* External function required by the framework. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t candidate = 5;          /* smallest number with both patterns */
    while (1) {
        if (has10and01(candidate)) {
            count++;
            if (count == n) return (seq_t)candidate;
        }
        candidate++;
    }
}
