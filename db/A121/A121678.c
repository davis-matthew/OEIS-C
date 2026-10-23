/*
 * A121678  a(n) = [x^n] (1 + x*(1+x)^n )^(n+1).
 * https://oeis.org/A121678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n+1} C(n+1,k) * C(n*k, n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n + 1; ++k)
    {
        seq_t c1 = binomial(n + 1, k);
        seq_t c2 = binomial(n * k, n - k);
        sum += c1 * c2;
    }
    return sum;
}
