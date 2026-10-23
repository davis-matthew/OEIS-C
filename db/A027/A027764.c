/*
 * A027764  a(n) = (n+1)*binomial(n+1,4).
 * https://oeis.org/A027764
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(n+1,4)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 159, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* offset starts at n = 3 */
    seq_t c = binomial(n + 1, 4); /* C(n+1,4) */
    return (seq_t)(n + 1) * c;    /* (n+1) * C(n+1,4) */
}
