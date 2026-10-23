/*
 * A161036  Number of partitions of n into Fibonacci numbers where every part
 *          appears at least 12 times.
 * https://oeis.org/A161036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into Fibonacci numbers where
 *            every summand appears at least 12 times. Computation is
 *            performed by dynamic programming over the Fibonacci numbers ≤ n.
 *            The algorithm runs in O(n log n) time and uses O(n) memory.
 *            Results are cached so that subsequent calls with large...
 * Range:     u8 n <= 111, u16 n <= 300, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* External integer types supplied by the build system. */
typedef uint64_t idx_t;   /* index type */

/* Sequence value type for arbitrary precision or standard integer. */
extern seq_t seq_add(seq_t a, seq_t b);

/* --------------------------------------------------------------------- */
/* Group: number of partitions using Fibonacci numbers with
   a minimum multiplicity of 12 for each used part. */
seq_t a(idx_t n)
{
    /* Return 0 for n == 0 (not used by OEIS). */
    if (n == 0) {
        return 0;
    }

    /* Cached DP array: dp_cache[i] = a(i) for i ≤ cache_max. */
    static seq_t *dp_cache = NULL;
    static idx_t cache_max = 0;

    /* If the cached array already covers the requested n, just return. */
    if (dp_cache && n <= cache_max) {
        return dp_cache[n];
    }

    /* Need to recompute up to n.  Allocate a new DP array. */
    size_t sz = (size_t)n + 1;          /* array size */
    seq_t *dp      = (seq_t *)calloc(sz, sizeof(seq_t)); /* dp[i] holds
                                                           a(i) for current
                                                           set of coins */
    if (!dp) {
        exit(EXIT_FAILURE);
    }
    dp[0] = 1;                           /* empty partition */

    /* Collect Fibonacci numbers ≤ n: 1, 2, 3, 5, 8, ... */
    idx_t fibs[32];
    size_t fibcnt = 0;
    idx_t a_fib = 1, b_fib = 2;
    fibs[fibcnt++] = a_fib;
    while (b_fib <= n) {
        fibs[fibcnt++] = b_fib;
        idx_t c = a_fib + b_fib;
        a_fib = b_fib;
        b_fib = c;
    }

    /* Temporary array for updates and a prefix array for each residue. */
    seq_t *dp_new = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp_new) {
        free(dp);
        exit(EXIT_FAILURE);
    }
    seq_t *pref = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!pref) {
        free(dp);
        free(dp_new);
        exit(EXIT_FAILURE);
    }

    /* Process each Fibonacci value. */
    for (size_t fi = 0; fi < fibcnt; ++fi) {
        idx_t F = fibs[fi];

        /* Start with the counts that do not use this part. */
        memcpy(dp_new, dp, sz * sizeof(seq_t));

        /* Consider residues modulo F. */
        for (idx_t r = 0; r < F && r <= n; ++r) {
            idx_t t_max = (n - r) / F;
            if (t_max < 12) {
                continue;   /* cannot use at least 12 copies */
            }

            /* Build prefix sums of dp over this residue. */
            pref[0] = dp[(size_t)r];
            for (idx_t t = 1; t <= t_max; ++t) {
                pref[t] = pref[t - 1] + dp[(size_t)(r + t * F)];
            }

            /* Add contributions for using t copies (t ≥ 12). */
            for (idx_t t = 12; t <= t_max; ++t) {
                size_t idx = (size_t)(r + t * F);
                dp_new[idx] += pref[t - 12];
            }
        }

        /* Prepare for the next coin: swap dp and dp_new. */
        seq_t *tmp = dp;
        dp = dp_new;
        dp_new = tmp;
    }

    /* Clean up temporary arrays. */
    free(dp_new);
    free(pref);

    /* Cache the computed array for future calls. */
    if (dp_cache) {
        free(dp_cache);
    }
    dp_cache = dp;
    cache_max = n;

    /* Return the requested value. */
    return dp_cache[n];
}
