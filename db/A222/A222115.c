/*
 * A222115  a(n) = 1 + Sum_{k=1..n} binomial(n,k) * sigma(k).
 * https://oeis.org/A222115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n} binomial(n,k) * sigma(k)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   _ensure_sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (a(1) is the first term)                                     */

#include <stdlib.h>   /* malloc, free */
#include "../utils/_ensure_sigma.h"

/* Public sequence function.  Computes a(n) for any n ≥ 1.         */
seq_t a(idx_t n)
{
    static seq_t *sigma = NULL;
    static idx_t sigma_size = 0;

    /* Grow the sigma table if needed. */
    _ensure_sigma(n, &sigma, &sigma_size);

    seq_t result = 1;      /* the constant "+1" in the definition      */
    seq_t binom  = 1;      /* C(n,0)                                 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update binomial coefficient multiplicatively.  All
           intermediate operations stay integer because (n-k+1) is
           divisible by k in the division.                 */
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;
        result += binom * sigma[k];
    }

    return result;
}
