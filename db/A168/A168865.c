/*
 * A168865  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n==0 else 44 * 43^(n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
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
        return (seq_t)1;
    return (seq_t)44 * ipow_idx((seq_t)43, n - 1);
}
