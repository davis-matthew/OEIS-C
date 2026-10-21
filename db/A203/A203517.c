/*
 * A203517  a(n) = A203516(n)/A000178(n).
 * https://oeis.org/A203517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^binomial(n,2) * Π_{j=0}^{n-1} binomial(2*j, j)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* offset: a(1) is the first term */

    seq_t result = (seq_t)1;
    seq_t two_power = (seq_t)1;   /* 2^j, increments each loop        */

    for (idx_t j = 0; j < n; ++j) {
        result *= binomial((idx_t)(2 * j), j);   /* multiply by binomial(2*j, j) */
        result *= two_power;                    /* multiply by 2^j                */
        two_power *= (seq_t)2;                  /* advance to next power of two  */
    }

    return result;
}
