/*
 * A000920  Differences of 0: 6!*Stirling2(n,6).
 * https://oeis.org/A000920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum((-1)^i*binomial(6, i)*(6-i)^n, i = 0 .. 5)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power.h"

seq_t a(idx_t n) {
    if (n < 6) return 0;
    
    seq_t binom[6] = {1, 6, 15, 20, 15, 6};
    seq_t result = 0;
    
    for (int i = 0; i < 6; i++) {
        seq_t pow_term = power((seq_t)(6 - i), n);
        seq_t term = binom[i] * pow_term;
        
        if (i % 2 == 0) {
            result += term;
        } else {
            result -= term;
        }
    }
    
    return result;
}
