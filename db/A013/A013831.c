/*
 * A013831  a(n) = 4^(5n+2).
 * https://oeis.org/A013831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(5n+2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 69 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* For the exponent 5n+2 the intermediate value fits
       comfortably in an unsigned 64‑bit integer. */
    idx_t exp = 5 * n + 2;
    /* pow_uint is a generic integer‑power routine provided by the
       framework; it accepts a 64‑bit base and a 64‑bit exponent and
       returns a value of type seq_t.  It works for every compiler
       configuration (8, 16, 32, 64, 128‑bit uints or arbitrary
       precision). */
    return ipow_u64_idx(4, exp);
}
