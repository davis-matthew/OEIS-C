/*
 * A002251  Start with the nonnegative integers; then swap L(k) and U(k) for
 *          all k >= 1, where L = A000201, U = A001950 (lower and upper
 *          Wythoff sequences).
 * https://oeis.org/A002251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Wythoff sequence swap. For each Wythoff pair (L(k), U(k)), where
 *            L(k) = floor(k*φ) and U(k) = floor(k*φ²), swap their values:
 *            a(L(k)) = U(k) and a(U(k)) = L(k)
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>


seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    const double phi = (1.0 + sqrt(5.0)) / 2.0;
    const double phi2 = phi + 1.0;
    const double inv_phi = 1.0 / phi;
    const double inv_phi2 = 1.0 / phi2;
    
    // Try to find k such that L(k) = floor(k*φ) = n
    double k_est_L = n * inv_phi;
    for (int dk = -3; dk <= 3; dk++) {
        idx_t k = (idx_t)(k_est_L + dk);
        if (k > 0) {
            idx_t L_k = (idx_t)(k * phi);
            if (L_k == n) {
                idx_t U_k = (idx_t)(k * phi2);
                return (seq_t)U_k;
            }
        }
    }
    
    // Try to find k such that U(k) = floor(k*φ²) = n
    double k_est_U = n * inv_phi2;
    for (int dk = -3; dk <= 3; dk++) {
        idx_t k = (idx_t)(k_est_U + dk);
        if (k > 0) {
            idx_t U_k = (idx_t)(k * phi2);
            if (U_k == n) {
                idx_t L_k = (idx_t)(k * phi);
                return (seq_t)L_k;
            }
        }
    }
    
    return (seq_t)n;
}
