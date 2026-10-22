/*
 * A006381  Number of n X 3 binary matrices under row and column permutations
 *          and column complementations.
 * https://oeis.org/A006381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (1/(1-x)^8 + 13/(1-x^2)^4 + 6/(1-x)^4/(1-x^2)^2 +
 *            12/(1-x^4)^2 + 8/(1-x)^2/(1-x^3)^2 + 8/(1-x^2)/(1-x^6))/48
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 71, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a000707_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/a000707_binom.h"


static seq_t coeff_1minus_pow(idx_t n, int d, int k) {
    if (n % d != 0) return 0;
    idx_t m = n / d;
    return a000707_binom(m + k - 1, k - 1);
}

static seq_t coeff_product2(idx_t n, int d1, int k1, int d2, int k2) {
    seq_t result = 0;
    for (idx_t i = 0; i <= n; i++) {
        result += coeff_1minus_pow(i, d1, k1) * coeff_1minus_pow(n - i, d2, k2);
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    
    result += coeff_1minus_pow(n, 1, 8);
    result += (seq_t)13 * coeff_1minus_pow(n, 2, 4);
    result += (seq_t)6 * coeff_product2(n, 1, 4, 2, 2);
    result += (seq_t)12 * coeff_1minus_pow(n, 4, 2);
    result += (seq_t)8 * coeff_product2(n, 1, 2, 3, 2);
    result += (seq_t)8 * coeff_product2(n, 2, 1, 6, 1);
    
    result /= (seq_t)48;
    
    return result;
}
