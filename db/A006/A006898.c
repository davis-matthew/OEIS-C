/*
 * A006898  a(n) = Sum_{k=0..n} C(n,k)*2^(k*(k+1)/2).
 * https://oeis.org/A006898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)*2^(k*(k+1)/2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   A001832_pow2, a000707_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"
#include "../utils/a000707_binom.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t k = 0; k <= n; k++) {
        idx_t exponent = k * (k + 1) / 2;
        seq_t binom = a000707_binom(n, k);
        seq_t power = A001832_pow2(exponent);
        result += binom * power;
    }
    return result;
}
