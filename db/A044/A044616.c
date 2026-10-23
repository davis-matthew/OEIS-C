/*
 * A044616  Numbers n such that string 6,0 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_valid_60
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for numbers whose base‑8 representation
   contains the substring 60 but does not contain it after adding 1. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_valid_60.h"

seq_t a(idx_t n)
{
    seq_t candidate = 0;
    idx_t count    = 0;

    while (1) {
        ++candidate;
        if (has_valid_60(candidate)) {
            ++count;
            if (count == n) {
                return candidate;
            }
        }
    }
}
