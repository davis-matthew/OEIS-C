/*
 * A186925  Coefficient of x^n in (1+n*x+x^2)^n.
 * https://oeis.org/A186925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{⌊n/2⌋} binomial(n,k) * binomial(n-k, n-2k) *
 *            n^(n-2k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = 0;
    idx_t half = n / 2;
    for (idx_t k = 0; k <= half; ++k)
    {
        seq_t term = binom(n, k);
        term *= binom(n - k, n - 2 * k);
        term *= big_pow((seq_t)n, (seq_t)(n - 2 * k));
        result += term;
    }
    return result;
}
