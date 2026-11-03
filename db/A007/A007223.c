/*
 * A007223  Number of distinct perforation patterns for deriving (v,b) =
 *          (n+2,n) punctured convolutional codes from (2,1).
 * https://oeis.org/A007223
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n)*(binomial(2*n, n+2) + [(n mod 2) == 0]*binomial(n,
 *            (n/2) + 1))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 26 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t result = binomial(2*n, n+2);
    
    if (n % 2 == 0) {
        result += binomial(n, n/2 + 1);
    }
    
    return result / (seq_t)n;
}
