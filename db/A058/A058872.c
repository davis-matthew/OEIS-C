/*
 * A058872  Number of 2-colored labeled graphs with n nodes.
 * https://oeis.org/A058872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (∑_{k=1}^{n-1} binomial(n,k) * 2^(k*(n-k))) / 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k) {
        seq_t bin = binomial(n, k);
        idx_t e = k * (n - k);
        seq_t pow2 = seq_pow((seq_t)2, e);
        sum += bin * pow2;
    }
    return sum / (seq_t)2;
}
