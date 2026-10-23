/*
 * A027796  a(n) = 55*(n+1)*binomial(n+3,11)/3.
 * https://oeis.org/A027796
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+1,9) * C(n+3,3)
 * Range:     u8 none, u16 n <= 10, u32 n <= 23, u64 n <= 137, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 8 – values below that are unspecified */
    if (n < 8) return 0;

    /* Compute the two binomial coefficients */
    seq_t c1 = binomial(n + 1, 9);
    seq_t c2 = binomial(n + 3, 3);

    /* Multiply them to obtain the sequence value */
    return c1 * c2;
}
