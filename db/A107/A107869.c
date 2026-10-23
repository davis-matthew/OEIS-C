/*
 * A107869  Column 1 of triangle A107867; a(n) = binomial( n*(n+1)/2 + n+1,
 *          n).
 * https://oeis.org/A107869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n*(n+1)/2 + n+1, n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* the binomial coefficient is defined for n=0 as binomial(1,0)=1 */
    if (n == 0)
        return 1;
    idx_t t = (idx_t)n * ((idx_t)n + 1) / 2 + n + 1;
    return binomial(t, n);
}
