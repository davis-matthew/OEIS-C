/*
 * A029193  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^6)*(1-x^7)).
 * https://oeis.org/A029193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = number of partitions of n into the parts 2, 5, 6 and 7.

   Recurrence (coin‑change DP):
   a(0) = 1, a(1) = 0; for n ≥ 2
   a(n) = Σ a(n – p) over all p ∈ {2,5,6,7} with n ≥ p.
   Each partition is counted once by processing the parts in increasing
   order.
*/
#include <stdlib.h>   /* calloc, free, NULL */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)     /* a(0) = 1 */
        return (seq_t)1;

    /* Allocate an array of size n+1, zero‑initialized.  In C++ the ‘new’
       operator automatically value‑initialises scalar types, which is
       equivalent to zero‑initialisation.  In plain C we use calloc. */
#ifdef __cplusplus
    seq_t *dp = new seq_t[n + 1];
#else
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
#endif
    if (!dp)   /* Allocation failure – return 0 (unlikely in the tests). */
        return (seq_t)0;

    dp[0] = (seq_t)1;   /* one way to make 0 – use no parts */

    /* Allowed part sizes, sorted increasingly. */
    static const idx_t parts[4] = {2, 5, 6, 7};

    /* Typical coin‑change DP: for each part add its contributions to all
       results greater than or equal to that part. */
    for (size_t k = 0; k < 4; ++k) {
        idx_t p = parts[k];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];

    /* Clean up. */
#ifdef __cplusplus
    delete [] dp;
#else
    free(dp);
#endif

    return result;
}
