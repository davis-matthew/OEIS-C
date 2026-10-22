/*
 * A003830  Order of universal Chevalley group D_n (3).
 * https://oeis.org/A003830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = D(3,n) where D(q,n) = q^(n*(n-1)) * (q^n - 1) *
 *            Product_{k=1..n-1}(q^(2*k) - 1), q=3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    idx_t exp1 = n * (n - 1);
    seq_t result = a000248_pow((seq_t)3, exp1);
    
    seq_t power_3n = a000248_pow((seq_t)3, n);
    result = result * (power_3n - 1);
    
    for (idx_t k = 1; k < n; k++) {
        seq_t term = a000248_pow((seq_t)3, 2 * k);
        result = result * (term - 1);
    }
    
    return result;
}
