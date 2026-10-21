/*
 * A230323  E.g.f.: Sum_{n>=0} x^n/n! * LambertW(-n*x)/(-n*x).
 * https://oeis.org/A230323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * (n-k)^k * (k+1)^(k-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin = binomial(n, k);
        seq_t part1 = 1;
        seq_t part2 = 1;
        if (k > 0)
        {
            part1 = big_pow((seq_t)(n - k), (seq_t)k);
            part2 = big_pow((seq_t)(k + 1), (seq_t)(k - 1));
        }
        seq_t term = bin * part1 * part2;
        sum += term;
    }
    return sum;
}
