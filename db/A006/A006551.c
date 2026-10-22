/*
 * A006551  Maximal Eulerian numbers.
 * https://oeis.org/A006551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{0<=j<=floor(n/2)} (-1)^j binomial(n+1,j)
 *            (floor(n/2)-j+1)^n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"


static seq_t ipow(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t k = n / 2;
    
    for (idx_t j = 0; j <= k; j++) {
        seq_t binom_coeff = binom(n + 1, j);
        seq_t base = (seq_t)(k - j + 1);
        seq_t power = ipow(base, n);
        seq_t term = binom_coeff * power;
        
        if (j % 2 == 0) {
            result += term;
        } else {
            result -= term;
        }
    }
    
    return result;
}
