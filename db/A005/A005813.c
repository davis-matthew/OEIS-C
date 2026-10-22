/*
 * A005813  Molien series for 6-dimensional complex representation of double
 *          cover of J2.
 * https://oeis.org/A005813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f.: (1 -x^3 -x^4 +x^10 +x^11 +x^12 +x^16 -x^19 -x^23 +x^26
 *            +x^30 +x^31 +x^32 -x^38 -x^39
 *            +x^42)/((1-x^3)*(1-x^4)*(1-x^6)*(1-x^7)*(1-x^10)*(1-x^15))
 * Range:     u8 n <= 53, u16 n <= 169, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
        
    // Compute coefficients of Q(x) = (1-x^3)*(1-x^4)*(1-x^6)*(1-x^7)*(1-x^10)*(1-x^15)
    idx_t max_q_degree = 45;
    seq_t *q = (seq_t *)calloc(max_q_degree + 1, sizeof(seq_t));
    q[0] = 1;
    
    // Multiply by (1-x^k) for k in {3, 4, 6, 7, 10, 15}
    idx_t k_vals[] = {3, 4, 6, 7, 10, 15};
    for (int k_idx = 0; k_idx < 6; k_idx++) {
        idx_t k = k_vals[k_idx];
        for (idx_t i = max_q_degree; i >= k; i--) {
            q[i] -= q[i - k];
        }
    }
    
    // Compute a(n) using recurrence: a(n) = p(n) - sum_{j=1}^{min(n, max_q_degree)} a(n-j) * q(j)
    seq_t *a_vals = (seq_t *)calloc(n + 1, sizeof(seq_t));
    
    for (idx_t i = 0; i <= n; i++) {
        // Coefficient of x^i in P(x) = 1 -x^3 -x^4 +x^10 +x^11 +x^12 +x^16 -x^19 -x^23 +x^26 +x^30 +x^31 +x^32 -x^38 -x^39 +x^42
        seq_t p_i;
        if (i == 0) p_i = 1;
        else if (i == 3 || i == 4 || i == 19 || i == 23 || i == 38 || i == 39) p_i = (seq_t)-1;
        else if (i == 10 || i == 11 || i == 12 || i == 16 || i == 26 || i == 30 || i == 31 || i == 32 || i == 42) p_i = 1;
        else p_i = 0;
        
        // Compute sum
        seq_t sum = 0;
        idx_t j_max = (i < max_q_degree) ? i : max_q_degree;
        for (idx_t j = 1; j <= j_max; j++) {
            sum += a_vals[i - j] * q[j];
        }
        
        a_vals[i] = p_i - sum;
    }
    
    seq_t result = a_vals[n];
    free(q);
    free(a_vals);
    return result;
}
