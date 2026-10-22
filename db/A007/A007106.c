/*
 * A007106  Number of labeled odd degree trees with 2n nodes.
 * https://oeis.org/A007106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Sum_{k=0..n-1} binomial(2*n,k) * (n-k)^(2*n-2)
 *            for n >= 2; a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   a000272_ipow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/a000272_ipow.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return 1;
    }
    
    seq_t result = 0;
    for (idx_t k = 0; k < n; k++) {
        seq_t binom_term = binom(2 * n, k);
        seq_t base = (seq_t)(n - k);
        seq_t pow_term = a000272_ipow(base, 2 * n - 2);
        result += binom_term * pow_term;
    }
    
    return result / 2;
}
