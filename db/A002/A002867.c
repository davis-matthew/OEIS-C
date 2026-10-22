/*
 * A002867  a(n) = binomial(n,floor(n/2))*(n+1)!.
 * https://oeis.org/A002867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n, floor(n/2)) * (n+1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   a000707_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000707_binom.h"

seq_t a(idx_t n) {
    idx_t k = n / 2;
    
    seq_t binom = a000707_binom(n, k);
    
    seq_t fact = 1;
    for (idx_t i = 2; i <= n + 1; i++) {
        fact *= (seq_t)i;
    }
    
    return binom * fact;
}
