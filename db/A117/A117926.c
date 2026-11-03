/*
 * A117926  a(n) = n^floor(sqrt(n)).
 * https://oeis.org/A117926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^floor(sqrt(n))
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 40, u64 n <= 99, u128 n <= 255,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 9696 (2026-11-01)
 * Helpers:   ipow_idx, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    idx_t e = isqrt(n);   /* floor(sqrt(n)) */
    return ipow_idx((seq_t)n, e);     /* n^e */
}
