/*
 * A128798  a(n) = n*(n-1)*4^n.
 * https://oeis.org/A128798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*4^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 977 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t nx = (seq_t)n;
    seq_t pow4 = ipow_i32((seq_t)4, (int)n);
    return nx * (nx - 1) * pow4;
}
