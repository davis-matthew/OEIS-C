/*
 * A126923  Coordination sequence for 20-dimensional cyclotomic lattice
 *          Z[zeta_33].
 * https://oeis.org/A126923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{min(20,n)} p[i] * binom(n-i+19, 19) where p =
 *            [1,13,91,444,1677,5187,13614,31083,61422,100561,126214,
 *            100561,61422,31083,13614,5187,1677,444,91,13,1]
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* coefficients of the numerator polynomial, 0 ≤ i ≤ 20 */
    static const seq_t p[21] = {
        1, 13, 91, 444, 1677, 5187, 13614, 31083, 61422, 100561,
        126214, 100561, 61422, 31083, 13614, 5187, 1677,
        444, 91, 13, 1
    };

    seq_t result = 0;
    idx_t limit = (n < 20) ? n : 20;

    for (idx_t i = 0; i <= limit; ++i)
    {
        /* binomial coefficient (n-i+19 choose 19) */
        seq_t bin = binom(n - i + 19, 19);
        result += bin * p[i];
    }
    return result;
}
