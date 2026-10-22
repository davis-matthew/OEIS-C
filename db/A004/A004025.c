/*
 * A004025  Theta series of b.c.c. lattice with respect to long edge.
 * https://oeis.org/A004025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   coefficient of q^n in 2*q*phi(q)*psi(q^4)^2, where phi(q) =
 *            sum_{m in Z} q^{m^2} and psi(q) = sum_{m>=0} q^{m(m+1)/2} are
 *            Ramanujan theta functions
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        memset(cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
        cache_size = new_size;
    }
    
    if (cache[n] != 0) {
        return cache[n];
    }
    
    idx_t target = n - 1;
    
    seq_t *phi = (seq_t *)calloc(target + 1, sizeof(seq_t));
    for (idx_t m = 0; m * m <= target; m++) {
        if (m == 0) {
            phi[0] = 1;
        } else {
            if (m * m <= target) phi[m * m] += 1;
            if (m * m <= target) phi[m * m] += 1;
        }
    }
    
    seq_t *psi_q4 = (seq_t *)calloc(target + 1, sizeof(seq_t));
    for (idx_t m = 0; 2 * m * (m + 1) <= target; m++) {
        psi_q4[2 * m * (m + 1)] = 1;
    }
    
    seq_t *psi_q4_sq = (seq_t *)calloc(target + 1, sizeof(seq_t));
    for (idx_t i = 0; i <= target; i++) {
        for (idx_t j = 0; i + j <= target; j++) {
            psi_q4_sq[i + j] += psi_q4[i] * psi_q4[j];
        }
    }
    
    seq_t product_coeff = 0;
    for (idx_t i = 0; i <= target; i++) {
        product_coeff += phi[i] * psi_q4_sq[target - i];
    }
    
    cache[n] = 2 * product_coeff;
    
    free(phi);
    free(psi_q4);
    free(psi_q4_sq);
    
    return cache[n];
}
