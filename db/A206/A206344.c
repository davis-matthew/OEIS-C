/*
 * A206344  a(n) = floor(n/2)^n.
 * https://oeis.org/A206344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2)^n
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 366 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* behavior below the offset is irrelevant */
    seq_t base = (seq_t)(n / 2);  /* floor division of n by 2 */
    return ipow_idx(base, n);
}
