/*
 * A155957  a(n) = (2*n^2)^n.
 * https://oeis.org/A155957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^2)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 175 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* 0^0 is defined as 1 */
    if (n == 0)
        return (seq_t)1;
    /* Compute 2*n^2 in seq_t to avoid overflow */
    seq_t nn   = (seq_t)n * (seq_t)n;
    seq_t base = (seq_t)2 * nn;
    /* Raise base to the n-th power */
    return ipow_idx(base, n);
}
