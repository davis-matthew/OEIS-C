/*
 * A108288  Main diagonal of table A060543; a(n) = C((n+1)^2-1, n*(n+1)).
 * https://oeis.org/A108288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial((n+1)^2 - 1, n*(n+1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* compute N = (n+1)^2 - 1 and K = n*(n+1) */
    idx_t np1 = n + 1;
    idx_t N = np1 * np1 - 1;
    idx_t K = n * np1;
    return binomial(N, K);
}
