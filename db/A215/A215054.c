/*
 * A215054  a(n) = 1/11*(binomial(n,11) - floor(n/11)).
 * https://oeis.org/A215054
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (binomial(n,11) - floor(n/11))/11
 * Range:     u8 n <= 13, u16 n <= 16, u32 n <= 34, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 11 binomial is undefined; returning 0 is harmless. */
    if (n < 11) {
        return 0;
    }

    /* Compute binomial(n,11) with seq_t arithmetic. */
    seq_t bin = binomial(n, 11);

    /* floor(n/11) as seq_t. */
    seq_t div11 = (seq_t)(n / 11);

    /* The difference is divisible by 11. */
    return (bin - div11) / 11;
}
