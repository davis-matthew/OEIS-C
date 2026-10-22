/*
 * A007223  Number of distinct perforation patterns for deriving (v,b) =
 *          (n+2,n) punctured convolutional codes from (2,1).
 * https://oeis.org/A007223
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n)*(binomial(2*n, n+2) + [(n mod 2) == 0]*binomial(n,
 *            (n/2) + 1))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    seq_t result = binom(2*n, n+2);
    
    if (n % 2 == 0) {
        result += binom(n, n/2 + 1);
    }
    
    return result / (seq_t)n;
}
