/*
 * A000984  Central binomial coefficients: binomial(2*n,n) = (2*n)!/(n!)^2.
 * https://oeis.org/A000984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice, walk, frac
 * Formula:   a(n) = Product_{k=1..n} (n+k)/k, a(0) = 1 (each partial product
 *            is binomial(n+k,k); gcd cancellation keeps intermediates <=
 *            a(n))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   cb_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cb_gcd_u64.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t k = 1; k <= n; k++) {
        uint64_t r = (uint64_t)(result % (seq_t)k);
        uint64_t g = cb_gcd_u64(k, r);
        uint64_t kk = (uint64_t)k / g;
        result /= (seq_t)g;
        result *= ((seq_t)n + k) / (seq_t)kk;
    }
    return result;
}
