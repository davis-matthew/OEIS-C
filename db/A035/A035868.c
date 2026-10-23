/*
 * A035868  Coordination sequence for A_42 lattice.
 * https://oeis.org/A035868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d=1..42} C(43, d)*C(n-1, d-1)*C(42-d+n, n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where n = 0 returns 1 separately.                            */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0) return (seq_t)1;

    seq_t result = (seq_t)0;
    idx_t n_minus1 = n - 1;

    for (idx_t d = 1; d <= 42; ++d) {
        if (d - 1 > n_minus1) continue;          /* C(n-1, d-1) = 0 */

        seq_t term1 = binomial((idx_t)43, d);
        seq_t term2 = binomial(n_minus1, d - 1);
        seq_t term3 = binomial(n + 42 - d, n);

        seq_t term = term1;
        term *= term2;
        term *= term3;
        result += term;
    }

    return result;
}
