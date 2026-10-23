/*
 * A169490  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A169490
   Formula:  a(0) = 1
             a(n) = 45·44^(n-1)  for n > 0
*/

#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    return pow_uint(44, n - 1) * (seq_t)45;
}
