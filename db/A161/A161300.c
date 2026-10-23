/*
 * A161300  Number of partitions of n into numbers not divisible by 4 where
 *          every part appears at least 9 times.
 * https://oeis.org/A161300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into numbers not divisible by 4
 *            where each part appears at least 9 times
 * Range:     u8 n <= 89, u16 n <= 217, u32 n <= 611, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Size for arrays (n can be > size_t on 32‑bit, but n ≤ 10000 here). */
    size_t N = (size_t)n;

    /* Allocate three working arrays. */
    seq_t *dp    = alloc_seq_array(N + 1); /* current DP state */
    seq_t *newdp = alloc_seq_array(N + 1); /* temporary DP state */
    seq_t *T     = alloc_seq_array(N + 1); /* running sums for (1‑p) knapsack */

    if (!dp || !newdp || !T)
    {
        /* Allocation failed – return 0 (safe fallback). */
        seq_free_array(dp);
        seq_free_array(newdp);
        seq_free_array(T);
        return (seq_t)0;
    }

    /* Base case: one way to make 0 with no parts. */
    dp[0] = (seq_t)1;

    /* Iterate over all admissible part sizes p (p % 4 != 0). */
    for (idx_t p = 1; p <= n; ++p)
    {
        if (p % 4 == 0)
            continue;                       /* skip multiples of 4 */

        size_t sp = (size_t)p;

        /* ---- Compute T: convolution of dp with unlimited copies of part p. */
        for (size_t s = 0; s <= N; ++s)
        {
            if (s < sp)
                T[s] = dp[s];
            else
            {
                T[s] = dp[s];
                T[s] += T[s - sp];
            }
        }

        /* ---- Build newdp: keep old ways (not using p) + ways using p at least 9 times. */
        size_t offset = 9 * sp;
        for (size_t s = 0; s <= N; ++s)
        {
            newdp[s] = dp[s];                 /* not using part p */

            if (s >= offset && offset <= N)
            {
                newdp[s] += T[s - offset];    /* use part p at least 9 times */
            }
        }

        /* Swap pointers for next iteration. */
        seq_t *tmp   = dp;
        dp          = newdp;
        newdp       = tmp;                   /* newdp will be overwritten next loop */
    }

    /* Result is the number of ways to make sum n. */
    seq_t answer = dp[N];

    /* Clean up. */
    seq_free_array(dp);
    seq_free_array(newdp);
    seq_free_array(T);

    return answer;
}
