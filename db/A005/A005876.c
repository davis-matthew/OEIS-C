/*
 * A005876  Theta series of cubic lattice with respect to edge.
 * https://oeis.org/A005876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * phi(q) * psi(q)^2, where phi and psi are Ramanujan
 *            theta functions phi(q) = sum_{k=-oo..oo} q^(k^2), psi(q) =
 *            sum_{k=0..oo} q^(k*(k+1)/2)
 * Range:     u8 n <= 441, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t *phi = (seq_t *)calloc(n + 1, sizeof(seq_t));
    for (idx_t k = 0; k * k <= n; k++) {
        if (k == 0) {
            phi[0] = 1;
        } else {
            phi[k * k] += 2;
        }
    }
    
    seq_t *psi = (seq_t *)calloc(n + 1, sizeof(seq_t));
    for (idx_t k = 0; k * (k + 1) / 2 <= n; k++) {
        psi[k * (k + 1) / 2] += 1;
    }
    
    seq_t *psi2 = (seq_t *)calloc(n + 1, sizeof(seq_t));
    for (idx_t i = 0; i <= n; i++) {
        for (idx_t j = 0; i + j <= n; j++) {
            psi2[i + j] += psi[i] * psi[j];
        }
    }
    
    seq_t *result_poly = (seq_t *)calloc(n + 1, sizeof(seq_t));
    for (idx_t i = 0; i <= n; i++) {
        for (idx_t j = 0; i + j <= n; j++) {
            result_poly[i + j] += phi[i] * psi2[j];
        }
    }
    
    seq_t result = 2 * result_poly[n];
    
    free(phi);
    free(psi);
    free(psi2);
    free(result_poly);
    
    return result;
}
