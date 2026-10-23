/*
 * A176758  a(n) = Sum_{k=0..floor((n-1)/2)} (3^k-1)*binomial(n, 2*k+1).
 * https://oeis.org/A176758
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} (3^k-1)*binomial(n,2*k+1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 3; for smaller n return 0 as a
       harmless placeholder. */
    if (n < 3)
        return 0;

    seq_t result = 0;
    seq_t pow3   = 1;   /* 3^0 */

    for (idx_t k = 0; 2*k + 1 <= n; ++k)
    {
        idx_t r   = 2*k + 1;
        seq_t bin = binomial(n, r);
        seq_t term = (pow3 - 1) * bin;
        result += term;
        pow3 *= 3;
    }

    return result;
}
