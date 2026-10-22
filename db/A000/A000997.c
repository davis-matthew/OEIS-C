/*
 * A000997  From a differential equation.
 * https://oeis.org/A000997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) = sum_{k=0}^{n-3} C(n-3,k)*a(k) for n >= 3, with a(0) = 0,
 *            a(1) = 1, a(2) = 0
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   a000707_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a000707_binom.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    
    seq_t *vals = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    
    vals[0] = 0;
    vals[1] = 1;
    vals[2] = 0;
    
    for (idx_t i = 3; i <= n; i++) {
        idx_t m = i - 3;
        seq_t sum = 0;
        
        for (idx_t k = 0; k <= m; k++) {
            seq_t binom = a000707_binom(m, k);
            sum += binom * vals[k];
        }
        
        vals[i] = sum;
    }
    
    seq_t result = vals[n];
    free(vals);
    return result;
}
