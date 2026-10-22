/*
 * A003510  An equivalence relation on permutations.
 * https://oeis.org/A003510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   E.g.f. exp(Sum_{j>=1} x^j / (j*phi(j))), where phi = Euler phi
 *            function
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   euler_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/euler_phi.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t *a_vals = (seq_t *)calloc(n + 1, sizeof(seq_t));
    a_vals[0] = 1;
    
    for (idx_t i = 1; i <= n; i++) {
        seq_t sum = 0;
        seq_t falling = 1;
        
        for (idx_t j = 1; j <= i; j++) {
            uint64_t phi_j = euler_phi(j);
            sum += (falling / (seq_t)phi_j) * a_vals[i - j];
            if (j < i)
                falling *= (seq_t)(i - j);
        }
        
        a_vals[i] = sum;
    }
    
    seq_t result = a_vals[n];
    free(a_vals);
    return result;
}
