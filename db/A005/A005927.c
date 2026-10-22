/*
 * A005927  Theta series of diamond with respect to deep hole.
 * https://oeis.org/A005927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Expansion of 4*q^3*psi^3(q^8) + (phi^3(q^4) - phi^3(-q^4))/2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    // Pattern: non-zero only at 8k+3 and 8k+4
    if ((n % 8 != 3) && (n % 8 != 4)) return 0;
    
    int maxdeg = (int)n;
    
    // Allocate power series arrays
    long long *phi_q4 = (long long *)calloc(maxdeg + 1, sizeof(long long));
    long long *psi_q8 = (long long *)calloc(maxdeg + 1, sizeof(long long));
    long long *phi_neg_q4 = (long long *)calloc(maxdeg + 1, sizeof(long long));
    
    // Compute phi(q^4) = 1 + 2*sum q^(4k^2)
    phi_q4[0] = 1;
    for (int k = 1; 4 * k * k <= maxdeg; k++) {
        phi_q4[4 * k * k] += 2;
    }
    
    // Compute phi(-q^4) = 1 + 2*sum (-1)^(k^2)*q^(4k^2)
    // (-1)^(k^2) = -1 when k is odd, +1 when k is even
    phi_neg_q4[0] = 1;
    for (int k = 1; 4 * k * k <= maxdeg; k++) {
        phi_neg_q4[4 * k * k] += (k % 2 == 0) ? 2 : -2;
    }
    
    // Compute psi(q^8) = sum q^(8*k(k+1)/2)
    for (int k = 0; 8 * k * (k + 1) / 2 <= maxdeg; k++) {
        psi_q8[8 * k * (k + 1) / 2] += 1;
    }
    
    // Helper arrays for polynomial squaring
    long long *phi_q4_sq = (long long *)calloc(maxdeg + 1, sizeof(long long));
    long long *phi_q4_cubed = (long long *)calloc(maxdeg + 1, sizeof(long long));
    long long *phi_neg_q4_sq = (long long *)calloc(maxdeg + 1, sizeof(long long));
    long long *phi_neg_q4_cubed = (long long *)calloc(maxdeg + 1, sizeof(long long));
    long long *psi_q8_sq = (long long *)calloc(maxdeg + 1, sizeof(long long));
    long long *psi_q8_cubed = (long long *)calloc(maxdeg + 1, sizeof(long long));
    
    // Compute phi(q^4)^2
    for (int i = 0; i <= maxdeg; i++) {
        for (int j = 0; i + j <= maxdeg; j++) {
            phi_q4_sq[i + j] += phi_q4[i] * phi_q4[j];
        }
    }
    
    // Compute phi(q^4)^3
    for (int i = 0; i <= maxdeg; i++) {
        for (int j = 0; i + j <= maxdeg; j++) {
            phi_q4_cubed[i + j] += phi_q4_sq[i] * phi_q4[j];
        }
    }
    
    // Compute phi(-q^4)^2
    for (int i = 0; i <= maxdeg; i++) {
        for (int j = 0; i + j <= maxdeg; j++) {
            phi_neg_q4_sq[i + j] += phi_neg_q4[i] * phi_neg_q4[j];
        }
    }
    
    // Compute phi(-q^4)^3
    for (int i = 0; i <= maxdeg; i++) {
        for (int j = 0; i + j <= maxdeg; j++) {
            phi_neg_q4_cubed[i + j] += phi_neg_q4_sq[i] * phi_neg_q4[j];
        }
    }
    
    // Compute psi(q^8)^2
    for (int i = 0; i <= maxdeg; i++) {
        for (int j = 0; i + j <= maxdeg; j++) {
            psi_q8_sq[i + j] += psi_q8[i] * psi_q8[j];
        }
    }
    
    // Compute psi(q^8)^3
    for (int i = 0; i <= maxdeg; i++) {
        for (int j = 0; i + j <= maxdeg; j++) {
            psi_q8_cubed[i + j] += psi_q8_sq[i] * psi_q8[j];
        }
    }
    
    long long result = 0;
    
    // First term: 4 * q^3 * psi^3(q^8)
    if (n >= 3) {
        result += 4 * psi_q8_cubed[n - 3];
    }
    
    // Second term: (phi^3(q^4) - phi^3(-q^4)) / 2
    result += (phi_q4_cubed[n] - phi_neg_q4_cubed[n]) / 2;
    
    free(phi_q4);
    free(psi_q8);
    free(phi_neg_q4);
    free(phi_q4_sq);
    free(phi_q4_cubed);
    free(phi_neg_q4_sq);
    free(phi_neg_q4_cubed);
    free(psi_q8_sq);
    free(psi_q8_cubed);
    
    return (seq_t)result;
}
