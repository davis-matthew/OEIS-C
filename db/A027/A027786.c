/*
 * A027786  a(n) = 13*(n+1)*binomial(n+2,13)/2.
 * https://oeis.org/A027786
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+1,12)*C(n+2,2)
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 25, u64 n <= 107,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 11) return 0;
    seq_t term1 = binomial(n + 1, 12);
    seq_t term2 = binomial(n + 2, 2);
    return term1 * term2;
}
