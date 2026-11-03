/*
 * A109493  a(n) = 7^((n^2 - n)/2).
 * https://oeis.org/A109493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^((n^2 - n)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    idx_t exp = n * (n - 1) / 2;  /* (n^2 - n)/2 using unsigned arithmetic */
    return ipow_idx((seq_t)7, exp);
}
