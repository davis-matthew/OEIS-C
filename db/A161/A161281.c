/*
 * A161281  Number of partitions of n into numbers not divisible by 3 where
 *          every part appears at least 3 times.
 * https://oeis.org/A161281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 119, u32 n <= 350, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A161281
 * Number of partitions of n into numbers not divisible by 3,
 * where every part appears at least 3 times. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *dp_cache = NULL;   /* cached DP array: a[0..size-1] */
static size_t dp_cache_size = 0; /* number of elements in dp_cache */

/* Compute DP values up to 'limit' (inclusive).  The array is stored
 * in dp_cache.  The routine uses two auxiliary buffers and swaps
 * them after each part.  All arithmetic is performed in seq_t. */
static void compute_dp_up_to(idx_t limit)
{
    size_t sz = (size_t)(limit + 1);
    seq_t *buf[2];
    buf[0] = (seq_t *)calloc(sz, sizeof(seq_t));
    buf[1] = (seq_t *)calloc(sz, sizeof(seq_t));
    buf[0][0] = (seq_t)1;            /* a(0) = 1 for the empty partition */

    int cur = 0, nxt = 1;

    for (idx_t p = 1; p <= limit; ++p) {
        if (p % 3 == 0) continue;   /* parts divisible by 3 are forbidden */

        /* compute buf[nxt] from buf[cur] */
        for (idx_t r = 0; r < p && r <= limit; ++r) {
            idx_t max_m = (limit - r) / p;
            seq_t sum = (seq_t)0;   /* Σ_{t=0}^{m-3} buf[cur][r + t*p] */
            for (idx_t m = 0; m <= max_m; ++m) {
                idx_t n = r + m * p;
                size_t idx = (size_t)n;
                if (m >= 3)
                    buf[nxt][idx] = buf[cur][idx] + sum;
                else
                    buf[nxt][idx] = buf[cur][idx];

                if (m >= 2) {
                    idx_t t = r + (m - 2) * p;
                    sum += buf[cur][(size_t)t];
                }
            }
        }

        /* swap roles of the buffers for the next part */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* buf[cur] now holds the final values */
    if (dp_cache) free(dp_cache);
    dp_cache = buf[cur];
    dp_cache_size = sz;

    /* the unused buffer gets freed automatically when its
       pointer is overwritten by the next call. */
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* sequence is defined for n ≥ 1 */

    /* precompute up to 10,000 (typical bound) or up to n if larger */
    idx_t needed = (n > 10000) ? n : 10000;
    if (dp_cache_size < (size_t)needed + 1)
        compute_dp_up_to(needed);

    return dp_cache[(size_t)n];
}
