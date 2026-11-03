/*
 * A017663  a(n) = (12*n + 11)^11.
 * https://oeis.org/A017663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 11)^11
 * Range:     u8 none, u16 none, u32 none, u64 n <= 3, u128 n <= 264,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 9941 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* Convert the index to the base value */
    seq_t base = (seq_t)n * 12 + 11;
    /* Raise to the 11th power */
    return ipow_idx(base, 11);
}
