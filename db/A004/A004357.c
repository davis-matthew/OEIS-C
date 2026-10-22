/*
 * A004357  Binomial coefficient C(6n,n-2).
 * https://oeis.org/A004357
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(6n, n-2), computed multiplicatively as prod_{j=0}^{n-3}
 *            (6n-j)/(j+1) with gcd reduction
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t k = n - 2;
    for (idx_t j = 0; j < k; j++) {
        seq_t num = (seq_t)6 * (seq_t)n - (seq_t)j;
        seq_t den = (seq_t)j + 1;
        seq_t g = gcd_seq(result, den);
        result /= g;
        den /= g;
        num /= den;
        result *= num;
    }
    return result;
}
