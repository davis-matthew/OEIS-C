/*
 * A164906  Duplicate of A056450.
 * https://oeis.org/A164906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2^(n + (n%2))
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* exponent = n + (n mod 2) */
    idx_t exp = n + (n & 1u);
    return ipow((seq_t)2, (seq_t)exp);
}
