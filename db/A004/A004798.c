/*
 * A004798  Convolution of Fibonacci numbers 1,2,3,5,... with themselves.
 * https://oeis.org/A004798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..floor((n+1)/2)} binomial(n+1-i,i)*(n-i)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 38, u64 n <= 83, u128 n <= 173,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t limit = (n + 1) / 2;
    
    for (idx_t i = 0; i <= limit; i++) {
        seq_t binom_val = binom(n + 1 - i, i);
        seq_t factor = (seq_t)(n - i);
        result += binom_val * factor;
    }
    
    return result;
}
