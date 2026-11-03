/*
 * A017375  a(n) = (10*n + 8)^11.
 * https://oeis.org/A017375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 8)^11
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4, u128 n <= 317,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 9361 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Compute the base (10*n + 8) as a seq_t, then raise it to the 11th power. */
    seq_t base = (seq_t)n * 10 + 8;
    return ipow(base, 11);
}
