/*
 * A211859  Number of partitions of n into parts <= 4 with the property that
 *          all parts have distinct multiplicities.
 * https://oeis.org/A211859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{S⊆pairs} (-1)^|S| C_S(n)
 * Range:     u8 n <= 32, u16 n <= 212, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A211859 – number of partitions of n into parts ≤4
 *  where all positive multiplicities are distinct.
 *
 *  This implementation uses inclusion–exclusion over the equal‑multiplicity
 *  constraints.  For each subset of pairs of part sizes we compute the
 *  number of solutions to the partition equation forced by that subset,
 *  shifting the equation to enforce positive multiplicities for every
 *  connected component.  The coefficient is obtained by an unlimited
 *  coin‑change DP.  The final answer is the alternating sum over all
 *  2⁶ subsets.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*   where pairs = {(1,2),(1,3),(1,4),(2,3),(2,4),(3,4)} and C_S(n) is  */
/*   the number of solutions of Σ_i i·m_i = n with the equalities in S   */

seq_t a(idx_t n)
{
    /* n=0 has exactly one partition (the empty one). */
    if (n == 0)
        return (seq_t)1;

    /* pair indices (0‑based for part sizes 1‑4) */
    const int pair_i[6] = {0, 0, 0, 1, 1, 2};
    const int pair_j[6] = {1, 2, 3, 2, 3, 3};

    /* Allocate DP array once, size n+1. */
    size_t dp_size = (size_t)(n) + 1;
    seq_t *dp = (seq_t *)malloc(dp_size * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;           /* allocation failure – unlikely in tests */

    seq_t add = 0, sub = 0;

    /* Iterate over all subsets of the 6 unordered pairs. */
    for (int mask = 0; mask < 64; ++mask)
    {
        /* Find connected components induced by edges in 'mask'. */
        int visited[4] = {0};
        idx_t shift_sum = 0;     /* sum of comp sums for components ≥2 */
        seq_t weights[4];        /* weight of each component variable */
        int wcnt = 0;

        for (int i = 0; i < 4; ++i)
        {
            if (visited[i])
                continue;

            /* breadth‑first search to collect the component. */
            int comp_sum = 0, comp_size = 0;
            int stack[4];
            int sp = 0;
            stack[sp++] = i;
            visited[i] = 1;

            while (sp)
            {
                int v = stack[--sp];
                comp_sum += v + 1;  /* part size */
                ++comp_size;

                /* explore all pairs containing v */
                for (int e = 0; e < 6; ++e)
                {
                    if (!((mask >> e) & 1))
                        continue;
                    int a = pair_i[e], b = pair_j[e];
                    if (a != v && b != v)
                        continue;
                    int u = (a == v) ? b : a;
                    if (!visited[u])
                    {
                        visited[u] = 1;
                        stack[sp++] = u;
                    }
                }
            }

            if (comp_size >= 2)
            {
                shift_sum += (idx_t)comp_sum;
                weights[wcnt++] = (seq_t)comp_sum;
            }
            else
            {
                weights[wcnt++] = (seq_t)comp_sum;
            }
        }

        if (n < shift_sum)     /* RHS negative → no solutions */
            continue;

        idx_t RHS = n - shift_sum;

        /* Initialise DP: dp[0] = 1, others 0. */
        for (idx_t i = 0; i <= n; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* Unbounded knapsack DP for the weights. */
        for (int wi = 0; wi < wcnt; ++wi)
        {
            idx_t w = (idx_t)weights[wi];
            for (idx_t t = w; t <= RHS; ++t)
                dp[t] += dp[t - w];
        }

        seq_t count = dp[RHS];
        if (__builtin_popcount(mask) & 1)
            sub += count;
        else
            add += count;
    }

    free(dp);
    /* add ≥ sub for all n, so subtraction cannot underflow. */
    return add - sub;
}
