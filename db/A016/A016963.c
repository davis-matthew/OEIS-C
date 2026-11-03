/*
 * A016963  a(n) = (6*n + 4)^7.
 * https://oeis.org/A016963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 4)^7
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 93, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 972 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t x = n;        /* cast n to seq_t */
    x = x * 6 + 4;      /* (6*n + 4) */
    return ipow(x, 7);  /* raise to the 7th power */
}
