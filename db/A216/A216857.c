/*
 * A216857  Number of connected functions from {1,2,...,n} into a subset of
 *          {1,2,...,n} that have a fixed point summed over all subsets.
 * https://oeis.org/A216857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * k^(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t res = 0;
    seq_t exp = (seq_t)(n - 1);
    for (idx_t k = 1; k <= n; ++k) {
        seq_t bin = binomial(n, k);
        seq_t pw = big_pow((seq_t)k, exp);
        res += bin * pw;
    }
    return res;
}
