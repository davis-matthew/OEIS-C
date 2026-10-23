/*
 * A128614  Number of labeled plane trees with n edges in which no vertex has
 *          outdegree one.
 * https://oeis.org/A128614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*Sum_{k=1..n-1} binom(n+1,k)*binom(n-k-1,k-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined below the offset */

    /* k runs from 1 to floor(n/2); terms with k > floor(n/2) are zero */
    idx_t k_max = n / 2;
    if (k_max == 0)   /* n = 1 */
        return 0;

    seq_t sum   = 0;
    seq_t binom1 = (seq_t)(n + 1);          /* C(n+1,1)          */
    seq_t binom2 = 1;                       /* C(n-2,0)          */

    for (idx_t k = 1; k <= k_max; ++k) {
        /* add current term */
        sum += binom1 * binom2;

        if (k == k_max)
            break;   /* no more updates needed */

        /* update C(n+1, k+1) */
        binom1 = (binom1 * (seq_t)(n + 1 - k)) / (seq_t)(k + 1);

        /* update C(n-k-1, k) from C(n-k-2, k-1) */
        idx_t a = n - 2 * k;          /* a = n-2k   */
        idx_t b = n - 2 * k - 1;      /* b = n-2k-1 */

        seq_t numer = (seq_t)a * (seq_t)b;                /* (n-2k)(n-2k-1) */
        seq_t denom = (seq_t)k * (seq_t)(n - k - 1);      /* k(n-k-1)       */

        binom2 = (binom2 * numer) / denom;
    }

    seq_t fact = factorial(n);
    return fact * sum;
}
