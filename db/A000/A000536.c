/*
 * A000536  Number of 3-line Latin rectangles.
 * https://oeis.org/A000536
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=K(n,2), K(0,0)=1, K(n,0)=n*K(n-1,0)+(-1)^n,
 *            j*K(n,j)=n*(n+1-2*j)*K(n-1,j-1)+n*(n-1)*K(n-2,j-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    seq_t *K0 = (seq_t *)malloc((n+1) * sizeof(seq_t));
    seq_t *K1 = (seq_t *)malloc((n+1) * sizeof(seq_t));
    
    K0[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        K0[i] = ((seq_t)i) * K0[i-1];
        if (i & 1) {
            K0[i] -= 1;
        } else {
            K0[i] += 1;
        }
    }
    
    K1[0] = 0;
    if (n >= 1) K1[1] = 0;
    for (idx_t i = 2; i <= n; i++) {
        seq_t sum_K0 = K0[i-1] + K0[i-2];
        K1[i] = ((seq_t)i) * ((seq_t)(i-1)) * sum_K0;
    }
    
    seq_t K2;
    if (n < 2) {
        K2 = 0;
    } else {
        seq_t term1 = ((seq_t)n) * ((seq_t)(n-3)) * K1[n-1];
        seq_t term2 = ((seq_t)n) * ((seq_t)(n-1)) * K1[n-2];
        K2 = (term1 + term2) / 2;
    }
    
    free(K0);
    free(K1);
    
    return K2;
}
