/*
 * A191585  Central coefficients of the Riordan matrix (1/(1-3*x^2),x/(1-x))
 *          (A191582).
 * https://oeis.org/A191585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0..⌊n/2⌋} binomial(2*n-2*i-1, n-2*i) * 3^i
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sum   = 0;
    seq_t power = 1;
    idx_t lim   = n / 2;

    for (idx_t i = 0; i <= lim; ++i) {
        idx_t m = 2*n - 2*i - 1;
        idx_t k = n - 2*i;
        seq_t bin = binom(m, k);

        sum += bin * power;
        power *= 3;
    }
    return sum;
}
