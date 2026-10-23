/*
 * A154031  Number of planar triangular n X n X n nonnegative integer grids
 *          with every similarly oriented 4 X 4 X 4 subtriangle summing to 2.
 * https://oeis.org/A154031
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = - Σ_{d=4}^{min(n,10)} c_d, where c_d = {-55, -63, -20,
 *            41, 60, 16, 6} for d = 4..10
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;                      /* offset begins at 4 */

    static const int coeff[7] = { -55, -63, -20, 41, 60, 16, 6 };
    int64_t sum = 0;
    idx_t limit = n;
    if (limit > 10) limit = 10;

    for (idx_t d = 4; d <= limit; ++d)
        sum += coeff[d - 4];

    return (seq_t)(-sum);                      /* convert to positive result */
}
