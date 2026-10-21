/*
 * A233853  Number of (n+1)X(1+1) 0..7 arrays with every 2X2 subblock having
 *          the sum of the squares of all six edge and diagonal differences
 *          equal to 59.
 * https://oeis.org/A233853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of (n+1)×2 arrays over {0,…,7} for which every *
 *            2×2 subblock has the sum of the squares of its six
 *            edge‐and‑diagonal * differences equal 59. The value a(n) is the
 *            total number of such * arrays of height n+1. A dynamic
 *            programming approach over a graph * of 64 states is used. No
 *            har...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* adjacency matrix for the 64 states (pairs (x,y), 0≤x,y≤7) */
static bool adj[64][64] = {false};
static bool adj_built = false;

/* dynamic programming arrays for one step of the walk */
static seq_t cur_dp[64] = {0};
static seq_t next_dp[64] = {0};

/* cache for already computed values of a(n) */
static seq_t *cache = NULL;
static size_t cache_size = 0;      /* allocated size */
static idx_t computed_up_to = 0;   /* largest n for which cache[n] is valid */

/* Helper to build the adjacency matrix once. */
static void build_adj(void)
{
    for (int a = 0; a < 8; ++a)
        for (int b = 0; b < 8; ++b)
            for (int c = 0; c < 8; ++c)
                for (int d = 0; d < 8; ++d)
                {
                    int i = a * 8 + b;   /* state index for (a,b) */
                    int j = c * 8 + d;   /* state index for (c,d) */

                    /* compute six differences */
                    int diff[6] = {
                        a - c,     /* vertical left */
                        b - d,     /* vertical right */
                        a - b,     /* horizontal top */
                        c - d,     /* horizontal bottom */
                        a - d,     /* diagonal top‑right → bottom‑left */
                        b - c      /* diagonal top‑left → bottom‑right */
                    };
                    long long sumsq = 0;
                    for (int k = 0; k < 6; ++k)
                        sumsq += (long long)diff[k] * diff[k];

                    if (sumsq == 59) {
                        adj[i][j] = true;
                        adj[j][i] = true;   /* symmetry */
                    }
                }
    adj_built = true;
}

/* Ensure cache is large enough for indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n + 1 <= cache_size)
        return;
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();            /* out of memory */
    /* zero new entries to avoid garbage reads */
    if (new_size > cache_size)
        for (size_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;
    cache = new_cache;
    cache_size = new_size;
}

/* Sequence generator.  Called with n>=1.  Order of calls is arbitrary. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined by OEIS; return 0 for safety */
        return 0;

    if (!adj_built) {
        build_adj();
        /* initial dp for length 0: each state contributes one sequence */
        for (int i = 0; i < 64; ++i)
            cur_dp[i] = 1;
        computed_up_to = 0;
    }

    ensure_cache_size(n);

    /* extend precomputation up to required n */
    while (computed_up_to < n) {
        /* compute next_dp from cur_dp using adjacency */
        for (int j = 0; j < 64; ++j) next_dp[j] = 0;
        for (int i = 0; i < 64; ++i) {
            if (cur_dp[i] == 0) continue;
            for (int j = 0; j < 64; ++j)
                if (adj[i][j])
                    next_dp[j] += cur_dp[i];
        }

        /* total number of sequences of length (computed_up_to+2) */
        seq_t total = 0;
        for (int j = 0; j < 64; ++j)
            total += next_dp[j];

        cache[computed_up_to + 1] = total;

        /* copy for next iteration */
        for (int i = 0; i < 64; ++i)
            cur_dp[i] = next_dp[i];

        ++computed_up_to;
    }

    return cache[n];
}
