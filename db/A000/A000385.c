/*
 * A000385  Convolution of A000203 with itself.
 * https://oeis.org/A000385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = Sum_{k=1..n} A000203(k)*A000203(n-k+1)
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 2062, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sigma.h"

seq_t a(idx_t n) {
    seq_t *sigma_values = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    
    for (idx_t k = 1; k <= n; k++) {
        sigma_values[k] = sigma(k);
    }
    
    seq_t result = 0;
    for (idx_t k = 1; k <= n; k++) {
        result += sigma_values[k] * sigma_values[n - k + 1];
    }
    
    free(sigma_values);
    return result;
}
