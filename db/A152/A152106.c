/*
 * A152106  a(n) = (11^n + 7^n)/2.
 * https://oeis.org/A152106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11^n + 7^n)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 878 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t p11 = ipow_idx((seq_t)11, n);
    seq_t p7  = ipow_idx((seq_t)7, n);
    return (p11 + p7) >> 1;    /* divide by two, exact since sum is even */
}
