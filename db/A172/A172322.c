/*
 * A172322  Number of graphs with nonzero edge sets on an n X n square grid
 *          for n=(1,2,3,...).
 * https://oeis.org/A172322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n*(n-1)) - 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    idx_t exp = 2 * n * (n - 1);
    seq_t val = ipow_idx((seq_t)2, exp);
    return val - 1;
}
