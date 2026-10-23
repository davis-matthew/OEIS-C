/*
 * A044371  Numbers n such that string 3,9 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_39
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044371: numbers n such that the decimal representation of n
 * contains the substring "39" but the representation of n-1 does not. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/contains_39.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for index 0, return 0 */

    idx_t count = 0;
    seq_t candidate = 1;   /* start testing from 1 */

    for (;;) {
        if (contains_39(candidate) && !contains_39(candidate - 1)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
