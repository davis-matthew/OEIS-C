/*
 * A017639  a(n) = (12*n + 9)^11.
 * https://oeis.org/A017639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 9)^11
 * Range:     u8 none, u16 none, u32 none, u64 n <= 3, u128 n <= 264,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 9753 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Compute base = 12*n + 9 as seq_t to avoid overflow in intermediate
       types, then use the provided big_pow helper to raise it to the
       power 11. */
    seq_t base = (seq_t)12 * n + 9;
    return ipow(base, 11);
}
