/*
 * A168838  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n == 0, else 17 * 16^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    } else {
        seq_t pow16 = ipow_i32((seq_t)16, (int)(n - 1));
        return pow16 * (seq_t)17;
    }
}
