/*
 * A213571  Rectangular array: (row n) = b**c, where b(h) = h, c(h) =
 *          (n-1+h)^2, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = value at position n in the anti‑diagonal flattening of
 *            the rectangular array defined by T(n,k) = 2^(n+k+1) – 2^n*(k+2)
 *            – binomial(k+1,2)
 * Range:     u8 n <= 21, u16 n <= 105, u32 n <= 465, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 0‑based position in the sequence */
    idx_t pos = n - 1;

    /* Find the diagonal d (size d) containing pos.
       The cumulative number of terms up to diagonal d is d*(d+1)/2. */
    idx_t d = 1;
    idx_t cum = 0;
    while (cum + d <= pos) {
        cum += d;
        ++d;
    }

    /* Offset t within the diagonal (0 … d‑1) */
    idx_t t    = pos - cum;          /* 0‑based index inside the diagonal */
    idx_t ni   = t + 1;              /* row number in the array (n)   */
    idx_t ki   = d - t;              /* column number in the array (k) */

    /* Compute T(ni,ki) = 2^(ni+ki+1) - 2^ni*(ki+2) - binomial(ki+1,2) */
    seq_t p1   = pow2(ni + ki + 1);        /* 2^(ni+ki+1)         */
    seq_t p2   = pow2(ni);                 /* 2^ni                 */
    p2        *= (seq_t)(ki + 2);          /* 2^ni * (ki+2)        */
    seq_t tri  = (seq_t)(ki + 1) * (seq_t)ki / 2; /* binomial(ki+1,2)   */

    seq_t res  = p1 - p2 - tri;
    return res;
}
