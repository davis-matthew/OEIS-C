/*
 * A027798  a(n) = 26*(n+1)*binomial(n+3,13).
 * https://oeis.org/A027798
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+1,11) * C(n+3,3)
 * Range:     u8 none, u16 n <= 12, u32 n <= 22, u64 n <= 97, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 10) return 0;                /* Undefined below the offset */

    seq_t c1 = binomial(n + 1, 11);      /* C(n+1, 11) */
    seq_t c2 = binomial(n + 3, 3);       /* C(n+3, 3) */

    return c1 * c2;                      /* Exact product */
}
