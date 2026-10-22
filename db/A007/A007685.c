/*
 * A007685  a(n) = Product_{k=1..n} binomial(2*k,k).
 * https://oeis.org/A007685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} binomial(2*k,k), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t result = 1;
    for (idx_t k = 1; k <= n; k++) {
        result *= binom(2*k, k);
    }
    return result;
}
