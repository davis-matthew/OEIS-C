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
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A169490
   Formula:  a(0) = 1
             a(n) = 45·44^(n-1)  for n > 0
*/

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    return ipow_u64_idx(44, n - 1) * (seq_t)45;
}
