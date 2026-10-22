/*
 * A007706  a(n) = 1 + coefficient of x^n in Product_{k>=1} (1-x^k)
 *          (essentially the expansion of the Dedekind function eta(x)).
 * https://oeis.org/A007706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 + coefficient of x^n in Product_{k>=1} (1-x^k)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    long long *coeffs = (long long *)malloc(((long long)n + 1) * sizeof(long long));
    for (long long i = 0; i <= (long long)n; i++) {
        coeffs[i] = 0;
    }
    coeffs[0] = 1;
    
    for (idx_t k = 1; k <= n; k++) {
        for (idx_t i = n; i >= k; i--) {
            coeffs[(long long)i] -= coeffs[(long long)(i - k)];
        }
    }
    
    long long coeff_n = coeffs[(long long)n];
    free(coeffs);
    return (seq_t)(1 + coeff_n);
}
