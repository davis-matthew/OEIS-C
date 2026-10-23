/*
 * A138364  The number of Motzkin n-paths with exactly one flat step.
 * https://oeis.org/A138364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n, (n+1)/2) for n odd, 0 otherwise
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0)                     /* n even */
        return 0;
    idx_t k = (n + 1) / 2;                /* (n+1)/2 is an integer since n is odd */
    return binomial(n, k);
}
