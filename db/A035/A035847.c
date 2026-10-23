/*
 * A035847  Coordination sequence for A_21 lattice.
 * https://oeis.org/A035847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d=1..21} C(22,d) * C(n-1,d-1) * C(21-d+n,n) (for n ≥
 *            1), * a(0) = 1
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    for (idx_t d = 1; d <= 21; ++d)
    {
        seq_t c1 = binomial(22, d);
        seq_t c2 = binomial(n - 1, d - 1);
        seq_t c3 = binomial(21 - d + n, n);
        seq_t term = c1 * c2 * c3;
        sum += term;
    }
    return sum;
}
