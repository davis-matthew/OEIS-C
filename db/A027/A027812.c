/*
 * A027812  a(n) = 7*(n+1)*binomial(n+5,7).
 * https://oeis.org/A027812
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*(n+1)*binomial(n+5,7)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 34, u64 n <= 580, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    /* compute the binomial coefficient C(n+5,7) */
    seq_t bin = binomial(n + 5, 7);
    /* multiply by 7 and (n+1), ensuring seq_t operands are used */
    seq_t res = 7 * ((seq_t)(n + 1)) * bin;
    return res;
}
