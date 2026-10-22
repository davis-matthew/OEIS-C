/*
 * A001723  Generalized Stirling numbers, [n+8,8]_5.
 * https://oeis.org/A001723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3+k, 3)*5^k*c(n+3, k+3), where
 *            c(n,k) are unsigned Stirling numbers of the first kind
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   stirling1_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/stirling1_row.h"

seq_t a(idx_t n) {
        
    idx_t N = n + 3;
    seq_t *stirling = stirling1_row(N);
    
    seq_t result = 0;
    seq_t pow5 = 1;
    
    for (idx_t k = 0; k <= n; k++) {
        seq_t binom = ((seq_t)(3 + k) * (seq_t)(2 + k) * (seq_t)(1 + k)) / 6;
        seq_t term = binom * pow5 * stirling[k + 3];
        result = result + term;
        pow5 = pow5 * 5;
    }
    
    free(stirling);
    return result;
}
