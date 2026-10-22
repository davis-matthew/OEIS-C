/*
 * A005260  a(n) = Sum_{k = 0..n} binomial(n,k)^4.
 * https://oeis.org/A005260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n} binomial(n,k)^4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t k = 0; k <= n; k++) {
        seq_t binom_nk = binom(n, k);
        seq_t binom2 = binom_nk * binom_nk;
        seq_t binom4 = binom2 * binom2;
        result += binom4;
    }
    return result;
}
