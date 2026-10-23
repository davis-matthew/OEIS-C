/*
 * A027910  T(2n,n-2), T given by A027907.
 * https://oeis.org/A027910
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..floor((2*n-3)/2)} C(2*n, n-2-i) * C(n-2-i, i)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   offset 2  (a(2) is the first term) */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    idx_t limit = (2 * n - 3) / 2;
    seq_t sum = 0;

    for (idx_t i = 0; i <= limit; ++i)
    {
        seq_t term1 = binomial(2 * n, n - 2 - i);
        seq_t term2 = binomial(n - 2 - i, i);
        sum += term1 * term2;
    }

    return sum;
}
