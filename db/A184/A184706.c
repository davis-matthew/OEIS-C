/*
 * A184706  Number of strings of numbers x(i=1..6) in 0..n with sum i*x(i)
 *          equal to n*6.
 * https://oeis.org/A184706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 157, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A184706:  a(n) = number of 6‑tuples (x1,…,x6) with 0 ≤ xi ≤ n
   such that Σ i·xi = 6·n.
   Computation: bounded knapsack DP over the six coefficients. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                               /* offset = 1 */

    const idx_t target = 6U * n;                 /* maximum weighted sum */
    const size_t len   = (size_t)(target + 1);   /* array dimension */

    /* dp[j] = number of ways to obtain sum j using processed coefficients */
    seq_t *dp = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp)
        return 0;
    dp[0] = 1;                                   /* empty combination */

    for (idx_t coeff = 1; coeff <= 6; ++coeff)
    {
        seq_t *next = (seq_t *)calloc(len, sizeof(seq_t));
        if (!next) { free(dp); return 0; }

        /* process each residue class modulo coeff */
        for (idx_t r = 0; r < coeff; ++r)
        {
            seq_t window = 0;                     /* sliding‑window sum */
            for (idx_t j = r; j <= target; j += coeff)
            {
                /* add contribution from current index */
                window += dp[(size_t)j];

                /* remove terms that exceed the allowed window
                   of size n+1 on each residue class */
                if (j >= coeff * (n + 1))
                    window -= dp[(size_t)(j - coeff * (n + 1))];

                next[(size_t)j] = window;
            }
        }

        free(dp);
        dp = next;
    }

    seq_t result = dp[(size_t)target];
    free(dp);
    return result;
}
