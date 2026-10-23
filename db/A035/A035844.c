/*
 * A035844  Coordination sequence for A_18 lattice.
 * https://oeis.org/A035844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d=1..18} C(19,d)*C(n-1, d-1)*C(18-d+n, n) / where n
 *            is the index (a(0)=1)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(0) special case */
    if (n == 0)
        return (seq_t)1;

    /* Binomial coefficients C(19,d) for d = 0..18 */
    static const idx_t C19[19] = {
        1ULL, 19ULL, 171ULL, 969ULL, 3876ULL,
        11628ULL, 27132ULL, 50388ULL, 75582ULL,
        92378ULL, 92378ULL, 75582ULL, 50388ULL,
        27132ULL, 11628ULL, 3876ULL, 969ULL,
        171ULL, 19ULL
    };

    seq_t sum = 0;
    for (idx_t d = 1; d <= 18; ++d)
    {
        /* C(19,d) * C(n-1,d-1) */
        seq_t term = (seq_t)C19[d] * binom(n - 1, d - 1);
        /* * C(18-d+n, n) */
        term *= binom(18 - d + n, n);
        sum += term;
    }
    return sum;
}
