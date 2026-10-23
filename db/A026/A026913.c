/*
 * A026913  a(n) = T(2n-1,n-2), T given by A026907.
 * https://oeis.org/A026913
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n-1, n-2) + 3*binomial(2*n+3, n) - 18
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2. For n < 2 we return 0. */
    if (n < 2)
        return 0;

    seq_t t1 = binomial(2 * n - 1, n - 2);
    seq_t t2 = binomial(2 * n + 3, n);

    return t1 + (seq_t)3 * t2 - (seq_t)18;
}
