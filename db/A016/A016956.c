/*
 * A016956  a(n) = (6*n + 3)^12.
 * https://oeis.org/A016956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 3)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 6, u128 n <= 270,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 1859 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;     /* cast to seq_t */
    base = base * 6 + 3;       /* compute (6*n + 3) */
    return ipow_i32(base, 12);  /* raise to the 12th power */
}
