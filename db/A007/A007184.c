/*
 * A007184  a(n) = Product_{k=0..n-1} (2^(2^k - 1) + 1)^C(n,k).
 * https://oeis.org/A007184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n-1} (2^(2^k - 1) + 1)^C(n,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   a000248_pow, a000707_binom, a000740_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000740_pow2.h"
#include "../utils/a000707_binom.h"
#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t k = 0; k < n; k++) {
        if (k >= 64) break;
        idx_t pow2k = (idx_t)1 << k;
        seq_t base = a000740_pow2(pow2k - 1) + 1;
        seq_t binom = a000707_binom(n, k);
        result = result * a000248_pow(base, (idx_t)binom);
    }
    return result;
}
