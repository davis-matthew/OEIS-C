/*
 * A068409  a(n) = binomial(binomial(2*n,n),n).
 * https://oeis.org/A068409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( binomial(2*n, n), n )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 42 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* ---------- multiplicative binomial N choose k ----------
   N is a seq_t and k is an idx_t (k ≤ N).  The algorithm
   avoids any intermediate factorial, dividing at every step.  */
static seq_t
binomial_big(seq_t N, idx_t k)
{
    if (k == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* N - (k - i) == N - k + i   */
        seq_t term = N - (seq_t)(k - i);
        result = (result * term) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* first compute the central binomial coefficient C(2n, n) */
    seq_t central = binomial((idx_t)(2*n), n);
    /* then compute C(central, n) */
    return binomial_big(central, n);
}
