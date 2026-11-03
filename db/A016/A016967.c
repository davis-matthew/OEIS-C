/*
 * A016967  a(n) = (6*n + 4)^11.
 * https://oeis.org/A016967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 4)^11
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 8, u128 n <= 529,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 840 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Computes the OEIS sequence A016967: a(n) = (6*n + 4)^11 */
seq_t a(idx_t n)
{
    /* cast n to seq_t to avoid overflow of the intermediate value
       when seq_t supports arbitrary precision.  For fixed-size types
       the result will be undefined if it does not fit, but such cases
       are outside the test set. */
    seq_t base = (seq_t)n * 6 + 4;
    return ipow(base, 11);
}
