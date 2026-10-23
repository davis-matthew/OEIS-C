/*
 * A121674  a(n) = [x^n] (1 + x*(1+x)^n )^n.
 * https://oeis.org/A121674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binom(n,k) * binom(n*k, n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_n_k.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t c1 = binom_n_k(n, k);
        idx_t nk  = n * k;
        idx_t nmk = n - k;
        seq_t c2 = binom_n_k(nk, nmk);
        sum += c1 * c2;
    }
    return sum;
}
