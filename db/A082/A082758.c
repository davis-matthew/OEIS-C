/*
 * A082758  Sum of the squares of the trinomial coefficients (A027907).
 * https://oeis.org/A082758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n - k, k) * binomial(2*n, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t two_n = n + n;
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t b1 = binomial(two_n - k, k);
        seq_t b2 = binomial(two_n, k);
        result += b1 * b2;
    }
    return result;
}
