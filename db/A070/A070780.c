/*
 * A070780  a(n) = binomial((n+1)^2,n).
 * https://oeis.org/A070780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial((n+1)^2, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 100 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A070780(n) = C((n+1)^2, n) using the provided binomial routine. */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* (n + 1)^2 fits in idx_t for the expected n range. */
    idx_t N = (idx_t)(n + 1);
    N *= N;
    return binomial(N, n);
}
