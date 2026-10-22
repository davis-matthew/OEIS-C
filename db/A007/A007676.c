/*
 * A007676  Numerators of convergents to e.
 * https://oeis.org/A007676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, frac
 * Formula:   Numerators of convergents to e from its continued fraction [2;
 *            1, 2, 1, 1, 4, 1, 1, 6, 1, 1, 8, ...]
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 22, u64 n <= 40, u128 n <= 70,
 *            big any n
 * Helpers:   cf_e
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/cf_e.h"

seq_t a(idx_t n) {
    seq_t p_prev2 = 1;
    seq_t p_prev1 = 2;
    
    for (idx_t i = 1; i <= n; i++) {
        seq_t a_i = cf_e(i);
        seq_t p_curr = a_i * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }
    
    return p_prev1;
}
