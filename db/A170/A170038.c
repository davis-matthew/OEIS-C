/*
 * A170038  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 29 * 28^(n-1) for n >= 1 (OEIS A170038)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
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
    if (n == 0)
        return (seq_t)1;

    /* compute 28^(n-1) */
    seq_t pow28 = ipow_i32((seq_t)28, (int)(n - 1));

    /* multiply by 29 */
    return (seq_t)29 * pow28;
}
