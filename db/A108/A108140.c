/*
 * A108140  a(n) = 4*a(n-1) -3*a(n-2) -2*a(n-3) +a(n-4), n>8.
 * https://oeis.org/A108140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=0,
 * a(5)=4, a(6)=17, a(7)=55, a(8)=161,
 * a(n)=4*a(n-1)-3*a(n-2)-2*a(n-3)+a(n-4) for n>=9 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n < 9 */
    static const seq_t base[9] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)0, (seq_t)4, (seq_t)17, (seq_t)55,
        (seq_t)161
    };

    /* Memoization table */
    static seq_t *dp = NULL;      /* array of computed values */
    static idx_t dp_max = 0;      /* largest index currently computed */
    static idx_t dp_cap = 0;      /* allocated capacity */

    /* If the requested index is within the known base, just return it */
    if (n < 9)
        return base[n];

    /* If we have already computed up to n, return it immediately */
    if (n <= dp_max)
        return dp[n];

    /* Allocate or grow the memoization array as needed */
    if (dp == NULL) { /* First request >= 9 */
        dp_cap = n + 1;
        dp = (seq_t *)malloc((size_t)dp_cap * sizeof(seq_t));
        if (dp == NULL) /* allocation failure: fall back to base values */
            return base[n];
        /* Initialize the first 9 terms */
        for (idx_t i = 0; i < 9; ++i)
            dp[i] = base[i];
        dp_max = 8; /* Last computed index so far */
    }
    else if (dp_cap <= n) {
        /* Grow the array to accommodate the new index */
        dp_cap = n + 1;
        dp = (seq_t *)realloc(dp, (size_t)dp_cap * sizeof(seq_t));
        if (dp == NULL) /* allocation failure: use fallback */
            return base[n];
        /* Existing values remain; base terms are already set */
        /* dp_max stays unchanged until we compute new entries */
    }

    /* Compute the values from the next uncomputed index up to n */
    for (idx_t i = dp_max + 1; i <= n; ++i) {
        if (i < 9) { /* safety: should already be initialized */
            dp[i] = base[i];
            continue;
        }
        dp[i] = 4 * dp[i - 1] - 3 * dp[i - 2] - 2 * dp[i - 3] + dp[i - 4];
    }
    dp_max = n;
    return dp[n];
}
