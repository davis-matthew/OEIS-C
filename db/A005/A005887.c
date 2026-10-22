/*
 * A005887  Theta series of f.c.c. lattice with respect to octahedral hole.
 * https://oeis.org/A005887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Coefficient of q^(2n+1) in (phi^3(q) - phi^3(-q))/2 where phi(q)
 *            = sum_{k=-infty}^{infty} q^(k^2)
 * Range:     u8 n <= 133, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


seq_t a(idx_t n) {
    idx_t max_q = 2 * n + 1;
    
    int64_t *phi = (int64_t *)calloc(max_q + 1, sizeof(int64_t));
    int64_t *phi_neg = (int64_t *)calloc(max_q + 1, sizeof(int64_t));
    
    if (!phi || !phi_neg) {
        free(phi);
        free(phi_neg);
        return (seq_t)0;
    }
    
    phi[0] = 1;
    phi_neg[0] = 1;
    
    for (idx_t k = 1; k * k <= max_q; k++) {
        idx_t k2 = k * k;
        phi[k2] += 2;
        phi_neg[k2] += (k % 2 == 0) ? 2 : -2;
    }
    
    int64_t *phi2 = (int64_t *)calloc(max_q + 1, sizeof(int64_t));
    if (!phi2) {
        free(phi);
        free(phi_neg);
        return (seq_t)0;
    }
    
    for (idx_t i = 0; i <= max_q; i++) {
        for (idx_t j = 0; i + j <= max_q; j++) {
            phi2[i + j] += phi[i] * phi[j];
        }
    }
    
    int64_t *phi3 = (int64_t *)calloc(max_q + 1, sizeof(int64_t));
    if (!phi3) {
        free(phi);
        free(phi_neg);
        free(phi2);
        return (seq_t)0;
    }
    
    for (idx_t i = 0; i <= max_q; i++) {
        for (idx_t j = 0; i + j <= max_q; j++) {
            phi3[i + j] += phi2[i] * phi[j];
        }
    }
    
    int64_t *phi_neg2 = (int64_t *)calloc(max_q + 1, sizeof(int64_t));
    if (!phi_neg2) {
        free(phi);
        free(phi_neg);
        free(phi2);
        free(phi3);
        return (seq_t)0;
    }
    
    for (idx_t i = 0; i <= max_q; i++) {
        for (idx_t j = 0; i + j <= max_q; j++) {
            phi_neg2[i + j] += phi_neg[i] * phi_neg[j];
        }
    }
    
    int64_t *phi_neg3 = (int64_t *)calloc(max_q + 1, sizeof(int64_t));
    if (!phi_neg3) {
        free(phi);
        free(phi_neg);
        free(phi2);
        free(phi3);
        free(phi_neg2);
        return (seq_t)0;
    }
    
    for (idx_t i = 0; i <= max_q; i++) {
        for (idx_t j = 0; i + j <= max_q; j++) {
            phi_neg3[i + j] += phi_neg2[i] * phi_neg[j];
        }
    }
    
    int64_t diff = (phi3[max_q] - phi_neg3[max_q]) / 2;
    seq_t result = (seq_t)diff;
    
    free(phi);
    free(phi_neg);
    free(phi2);
    free(phi3);
    free(phi_neg2);
    free(phi_neg3);
    
    return result;
}
