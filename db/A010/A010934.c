/*
 * A010934  Binomial coefficient C(18,n).
 * https://oeis.org/A010934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial coefficient C(18, n)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n > 18u)
        return 0;
    return binomial(18, n);
}
