/*
 * A010957  Binomial coefficient C(41,n).
 * https://oeis.org/A010957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(41,n) (binomial coefficient of 41 choose n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n > 41U)                     /* n > 41 yields zero */
        return (seq_t)0;
    return binomial((idx_t)41, n);   /* compute 41 choose n */
}
