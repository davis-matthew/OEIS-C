/*
 * A045206  Numbers whose base-5 representation contains exactly one 0 and no
 *          4's.
 * https://oeis.org/A045206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   a045206_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045206: the n-th integer whose base‑5 representation
 * contains exactly one digit 0 and no digit 4, found by exhaustive search. */
#include <stdbool.h>
#include "../utils/a045206_valid.h"

seq_t a(idx_t n)
{
    idx_t counted = 0;
    seq_t val = (seq_t)0;
    while (true) {
        if (a045206_valid(val)) {
            ++counted;
            if (counted == n)
                return val;
        }
        ++val;
    }
}
