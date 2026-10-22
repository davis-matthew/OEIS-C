/*
 * A003781  Expansion of theta series of {E_7}* lattice in powers of q^(1/2).
 * https://oeis.org/A003781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Expansion of phi(q)^3 * (phi(q)^4 + 7 * phi(-q)^4) / 8 in powers
 *            of q
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdlib.h>

static long long *multiply_series(const long long *a, const long long *b, idx_t maxN) {
    long long *result = (long long *)calloc(maxN + 1, sizeof(long long));
    if (!result) return NULL;
    
    for (idx_t i = 0; i <= maxN; i++) {
        if (a[i] == 0) continue;
        for (idx_t j = 0; i + j <= maxN; j++) {
            if (b[j] == 0) continue;
            result[i + j] += a[i] * b[j];
        }
    }
    return result;
}

static seq_t compute_a(idx_t n) {
    long long *phi_q = (long long *)calloc(n + 1, sizeof(long long));
    long long *phi_neg_q = (long long *)calloc(n + 1, sizeof(long long));
    
    if (!phi_q || !phi_neg_q) {
        free(phi_q);
        free(phi_neg_q);
        return 0;
    }
    
    phi_q[0] = 1;
    phi_neg_q[0] = 1;
    
    for (idx_t k = 1; k * k <= n; k++) {
        idx_t sq = k * k;
        phi_q[sq] = 2;
        phi_neg_q[sq] = (k % 2 == 0) ? 2 : -2;
    }
    
    long long *phi_q2 = multiply_series(phi_q, phi_q, n);
    long long *phi_q3 = multiply_series(phi_q2, phi_q, n);
    long long *phi_q4 = multiply_series(phi_q2, phi_q2, n);
    
    long long *phi_neg_q2 = multiply_series(phi_neg_q, phi_neg_q, n);
    long long *phi_neg_q4 = multiply_series(phi_neg_q2, phi_neg_q2, n);
    
    long long *temp = (long long *)malloc((n + 1) * sizeof(long long));
    memcpy(temp, phi_q4, (n + 1) * sizeof(long long));
    for (idx_t i = 0; i <= n; i++) {
        temp[i] += 7 * phi_neg_q4[i];
    }
    
    long long *result_series = multiply_series(phi_q3, temp, n);
    
    long long coeff = result_series[n] / 8;
    seq_t result = (seq_t)coeff;
    
    free(phi_q);
    free(phi_neg_q);
    free(phi_q2);
    free(phi_q3);
    free(phi_q4);
    free(phi_neg_q2);
    free(phi_neg_q4);
    free(temp);
    free(result_series);
    
    return result;
}

seq_t a(idx_t n) {
    idx_t mod = n % 4;
    if (mod == 1 || mod == 2) {
        return 0;
    }
    return compute_a(n);
}
