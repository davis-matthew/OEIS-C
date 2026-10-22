/*
 * A006261  a(n) = Sum_{k=0..5} binomial(n,k).
 * https://oeis.org/A006261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..5} binomial(n,k), with binomial(n,k+1) =
 *            binomial(n,k)*(n-k)/(k+1)
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 220, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_div_exact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_div_exact.h"

seq_t a(idx_t n) {
    seq_t result = (seq_t)1;
    seq_t c = (seq_t)1;
    idx_t k;
    for (k = 0; k < 5 && k < n; k++) {
        c = mul_div_exact(c, (seq_t)(n - k), (seq_t)(k + 1));
        result += c;
    }
    return result;
}
