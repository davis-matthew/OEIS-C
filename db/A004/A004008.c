/*
 * A004008  Expansion of theta series of E_7 lattice in powers of q^2.
 * https://oeis.org/A004008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   phi(q)^3 * (phi(q)^4 + 112*q*(psi(q^2))^4) where phi and psi are
 *            Ramanujan theta functions
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 991, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void poly_mult(seq_t *result, const seq_t *a, const seq_t *b, idx_t max_degree) {
    for (idx_t i = 0; i <= max_degree; i++) {
        result[i] = 0;
    }
    for (idx_t i = 0; i <= max_degree; i++) {
        if (a[i] == 0) continue;
        for (idx_t j = 0; i + j <= max_degree; j++) {
            if (b[j] == 0) continue;
            result[i + j] += a[i] * b[j];
        }
    }
}

static void compute_cache_to(idx_t n) {
    seq_t *phi = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *psi_q2 = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *phi2 = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *phi3 = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *phi4 = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *psi2 = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *psi4 = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *factor2 = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *result = (seq_t *)calloc(n + 1, sizeof(seq_t));
    
    // phi(q) = sum_{k=-inf}^{inf} q^(k^2)
    for (idx_t k = 0; k * k <= n; k++) {
        if (k == 0) {
            phi[0] += 1;
        } else {
            phi[k * k] += 2;
        }
    }
    
    // psi(q^2) = sum_{k=0}^{inf} q^(k(k+1))
    for (idx_t k = 0; k * (k + 1) <= n; k++) {
        psi_q2[k * (k + 1)] += 1;
    }
    
    poly_mult(phi2, phi, phi, n);
    poly_mult(phi3, phi2, phi, n);
    poly_mult(phi4, phi2, phi2, n);
    poly_mult(psi2, psi_q2, psi_q2, n);
    poly_mult(psi4, psi2, psi2, n);
    
    // factor2 = phi^4 + 112*q*psi4
    for (idx_t i = 0; i <= n; i++) {
        factor2[i] = phi4[i];
    }
    for (idx_t i = 1; i <= n; i++) {
        factor2[i] += (seq_t)112 * psi4[i - 1];
    }
    
    poly_mult(result, phi3, factor2, n);
    
    if (cache) free(cache);
    cache = result;
    cache_len = n + 1;
    
    free(phi); free(psi_q2); free(phi2); free(phi3); free(phi4);
    free(psi2); free(psi4); free(factor2);
}

seq_t a(idx_t n) {
    if (n >= cache_len) {
        compute_cache_to(n);
    }
    return cache[n];
}
