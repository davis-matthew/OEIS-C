/*
 * A007225  Number of distinct perforation patterns for deriving (v,b) =
 *          (n+4,n) punctured convolutional codes from (2,1).
 * https://oeis.org/A007225
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (C(2n,n+4) + [n even]*C(n,n/2+2) +
 *            2*[n%4==0]*C(n/2,n/4+1)) / n
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"


seq_t a(idx_t n) {
    if (n < 5) return 0;
    
    seq_t result = binom(2 * n, n + 4);
    
    if (n % 2 == 0) {
        result = result + binom(n, n / 2 + 2);
    }
    
    if (n % 4 == 0) {
        result = result + 2 * binom(n / 2, n / 4 + 1);
    }
    
    result = result / (seq_t)n;
    
    return result;
}
