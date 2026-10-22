/*
 * A004403  Expansion of 1/theta_3(q)^2 in powers of q.
 * https://oeis.org/A004403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of q^n in 1/(theta_3(q))^2, where theta_3(q)
 *            = Sum_{k=-∞}^{∞} q^(k^2)
 * Range:     i8 n <= 4, i16 n <= 13, i32 n <= 42, i64 n <= 139, i128 n <= 490,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t len = n + 1;
    
    seq_t *theta3 = (seq_t *)calloc(len, sizeof(seq_t));
    if (!theta3) return 0;
    
    theta3[0] = 1;
    for (idx_t k = 1; k * k <= n; k++) {
        theta3[k * k] = 2;
    }
    
    seq_t *theta3_sq = (seq_t *)calloc(len, sizeof(seq_t));
    if (!theta3_sq) {
        free(theta3);
        return 0;
    }
    
    for (idx_t i = 0; i <= n; i++) {
        if (theta3[i] != 0) {
            for (idx_t j = 0; i + j <= n; j++) {
                theta3_sq[i + j] += theta3[i] * theta3[j];
            }
        }
    }
    
    seq_t *result = (seq_t *)calloc(len, sizeof(seq_t));
    if (!result) {
        free(theta3);
        free(theta3_sq);
        return 0;
    }
    
    result[0] = 1;
    
    for (idx_t k = 1; k <= n; k++) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= k; j++) {
            sum += result[k - j] * theta3_sq[j];
        }
        result[k] = -sum;
    }
    
    seq_t ans = result[n];
    free(theta3);
    free(theta3_sq);
    free(result);
    
    return ans;
}
