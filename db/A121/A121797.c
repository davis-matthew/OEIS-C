/*
 * A121797  Number of dissimilar forests on n nodes.
 * https://oeis.org/A121797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{m≥1}(1 + Catalan(m)·x^m)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here Catalan(m) = (1/(m+1))·binom(2m,m).  The code below
   implements this product by dynamic programming, using 128‑bit
   intermediates to avoid overflow when seq_t is an unsigned
   integer type (uint8_t … uint64_t).  If seq_t is an arbitrary‑precision
   integer class the C++17 build uses the class operators
   directly. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
/* C++17 path – use seq_t operators directly. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* allocate DP table */
    seq_t *dp = new seq_t[n + 1];
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    seq_t cat = (seq_t)1;   /* Cat(0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* add contributions of part size k */
        for (idx_t s = n; s >= k; --s)
        {
            dp[s] = dp[s] + dp[s - k] * cat;
        }

        /* compute next Catalan number */
        cat = cat * ((seq_t)(4 * k - 2)) / (seq_t)(k + 1);
    }

    seq_t result = dp[n];
    delete[] dp;
    return result;
}
#else
/* C11 path – assume seq_t is an unsigned integer type.
   Use unsigned __int128 for intermediate arithmetic. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* allocate DP table */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;   /* allocation failure */

    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    seq_t cat = (seq_t)1;   /* Cat(0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* update DP table for part size k, using 128‑bit intermediates */
        for (idx_t s = n; s >= k; --s)
        {
            unsigned __int128 prod = (unsigned __int128)dp[s - k] * cat;
            unsigned __int128 sum = (unsigned __int128)dp[s] + prod;
            dp[s] = (seq_t)sum;
        }

        /* compute next Catalan number Cat(k) from Cat(k‑1) */
        unsigned __int128 mult = (unsigned __int128)cat * (unsigned __int128)(4 * k - 2);
        cat = (seq_t)(mult / (unsigned __int128)(k + 1));
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
#endif
