/*
 * A016920  a(n) = (6*n)^12.
 * https://oeis.org/A016920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n)^12
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 6, u128 n <= 270,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 1756 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)n) * ((seq_t)6);
    return ipow(base, (seq_t)12);
}
