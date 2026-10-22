/*
 * A002002  a(n) = Sum_{k=0..n-1} binomial(n,k+1) * binomial(n+k,k).
 * https://oeis.org/A002002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n,k+1) * binomial(n+k,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   a000707_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000707_binom.h"


seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t k = 0; k < n; k++) {
        seq_t b1 = a000707_binom(n, k + 1);
        seq_t b2 = a000707_binom(n + k, k);
        result += b1 * b2;
    }
    return result;
}
