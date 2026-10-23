/*
 * A168932  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 0; otherwise a(n) = 15 * 14^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }

    seq_t result = 15;          /* 15 for the first generator */
    for (idx_t i = 1; i < n; ++i) {
        result = seq_mul(result, (seq_t)14);   /* multiply by 14 for each additional step */
    }
    return result;
}
