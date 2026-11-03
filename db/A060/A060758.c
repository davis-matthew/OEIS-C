/*
 * A060758  a(n) = 5^(n^2).
 * https://oeis.org/A060758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 36 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    /* exponent = n^2 (stored in seq_t to avoid overflow when seq_t is big) */
    seq_t exp = (seq_t)n * (seq_t)n;
    /* base = 5 as seq_t */
    seq_t base = 5;
    /* compute 5^exp */
    return ipow_i32(base, (idx_t)exp);
}
