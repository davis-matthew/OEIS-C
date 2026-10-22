/*
 * A007677  Denominators of convergents to e.
 * https://oeis.org/A007677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, frac
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 22, u64 n <= 40, u128 n <= 70,
 *            big any n
 * Helpers:   get_cf_coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_cf_coeff.h"
/* Recurrence: a(n) = cf[n] * a(n-1) + a(n-2), a(0) = 1, a(-1) = 0
   where cf = [2, 1, 2, 1, 1, 4, 1, 1, 6, ...] are partial quotients of e */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t q_prev2 = 0;
    seq_t q_prev1 = 1;
    
    for (idx_t i = 1; i <= n; i++) {
        seq_t coeff = get_cf_coeff(i);
        seq_t q_curr = coeff * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }
    
    return q_prev1;
}
