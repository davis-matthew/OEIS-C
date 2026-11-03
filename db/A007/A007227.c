/*
 * A007227  Number of distinct perforation patterns for deriving (v,b) =
 *          (n+2,n) punctured convolutional codes from (3,1).
 * https://oeis.org/A007227
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n)*(binomial(3*n, n+2) + [(n mod 2) =
 *            0]*binomial(3*n/2, (n/2) + 1))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 21 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t term1 = binomial(3*n, n+2);
    seq_t result = term1;
    
    if (n % 2 == 0) {
        seq_t term2 = binomial(3*n/2, n/2 + 1);
        result = result + term2;
    }
    
    result = result / (seq_t)n;
    
    return result;
}
