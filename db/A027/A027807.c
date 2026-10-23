/*
 * A027807  a(n) = 165*(n+1)*binomial(n+4,11)/4.
 * https://oeis.org/A027807
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1,8) * binomial(n+4,4)
 * Range:     u8 none, u16 n <= 9, u32 n <= 21, u64 n <= 128, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 7)
        return 0;
    return binomial(n + 1, 8) * binomial(n + 4, 4);
}
