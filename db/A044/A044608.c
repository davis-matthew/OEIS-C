/*
 * A044608  Numbers n such that string 5,0 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_50_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044608: returns the nth number n such that the base‑8
 * representation of n contains the substring “50” but the base‑8
 * representation of n+1 does not. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_50_oct.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 for safety.  The caller normally
     * supplies n >= 1 (the OEIS offset). */
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t found = 0;
    seq_t candidate = 0;

    while (found < n) {
        /* Test the conditions for the candidate. */
        if (has_50_oct(candidate) && !has_50_oct(candidate + (seq_t)1)) {
            ++found;
            if (found == n) {
                return candidate;
            }
        }
        ++candidate;
    }
    /* Unreachable for n >= 1, but keep a return to satisfy the compiler. */
    return (seq_t)0;
}
