/*
 * A138035  Binomial transform of A135416.
 * https://oeis.org/A138035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m≥1, 2^m−2 ≤ n} binomial(n, 2^m−2) * 2^{m−1}
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t power = 2, coeff = 1; ; power <<= 1, coeff <<= 1) {
        idx_t k = power - 2;
        if (k > n) break;
        sum += binomial(n, k) * coeff;
    }
    return sum;
}
