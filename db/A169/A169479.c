/*
 * A169479  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 34 * 33^(n-1) for n >= 1, a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    /* Compute 33^(n-1) using the provided utility function */
    seq_t term = ipow_i32((seq_t)33, (int)(n - 1));
    /* Scale by 34 */
    return term * (seq_t)34;
}
