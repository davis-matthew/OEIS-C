/*
 * A070781  a(n) = binomial((n+1)^2, n^2).
 * https://oeis.org/A070781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial((n+1)^2, n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute (n+1)^2 and n^2 avoiding overflow for the intended test range (n <= 10000). */
    idx_t N = (n + 1) * (n + 1);   /* Upper bound of the binomial */
    idx_t K = n * n;               /* Lower bound of the binomial */

    return binomial(N, K);         /* Use the provided binomial routine (multiplicative). */
}
