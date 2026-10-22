/*
 * A005404  Number of protruded partitions of n with largest part at most 3.
 * https://oeis.org/A005404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = (1-x)^3/Product(1-x-x^i+x^(1+2*i), i=1..3)-1 Computed via
 *            coefficient recurrence: h(n) from polynomial division, a(n)=h(n)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *h = NULL;
    static idx_t h_capacity = 0;
    static idx_t h_len = 0;
    
    if (h == NULL) {
        h_capacity = 1024;
        h = (seq_t *)malloc(h_capacity * sizeof(seq_t));
        for (idx_t i = 0; i < h_capacity; i++) h[i] = 0;
        h[0] = 1;
        h_len = 1;
    }
    
    while (n >= h_capacity) {
        h_capacity *= 2;
        seq_t *new_h = (seq_t *)malloc(h_capacity * sizeof(seq_t));
        for (idx_t i = 0; i < h_len; i++) new_h[i] = h[i];
        for (idx_t i = h_len; i < h_capacity; i++) new_h[i] = 0;
        free(h);
        h = new_h;
    }
    
    seq_t q[16] = {1, -4, 4, 1, -1, 0, -5, 4, -2, 2, 3, -2, 0, -2, 0, 1};
    seq_t binom_coeffs[4] = {1, -3, 3, -1};
    
    for (idx_t m = h_len; m <= n; m++) {
        seq_t coeff = (m < 4) ? binom_coeffs[m] : 0;
        
        seq_t sum = 0;
        for (idx_t k = 1; k <= m && k < 16; k++) {
            sum += q[k] * h[m - k];
        }
        
        h[m] = coeff - sum;
    }
    h_len = n + 1;
    
    return h[n];
}
