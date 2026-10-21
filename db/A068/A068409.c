/*
 * A068409  a(n) = binomial(binomial(2*n,n),n).
 * https://oeis.org/A068409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( binomial(2*n, n), n )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   binomial, binomial_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/binomial_big.h"

seq_t a(idx_t n)
{
    /* first compute the central binomial coefficient C(2n, n) */
    seq_t central = binomial((idx_t)(2*n), n);
    /* then compute C(central, n) */
    return binomial_big(central, n);
}
