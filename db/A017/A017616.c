/*
 * A017616  a(n) = (12*n + 7)^12.
 * https://oeis.org/A017616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 7)^12
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 134,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 9686 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Compute the base 12*n + 7 as seq_t and raise it to the power 12 */
    seq_t base = (seq_t)n * 12 + 7;
    return ipow(base, (seq_t)12);
}
