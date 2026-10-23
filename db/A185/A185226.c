/*
 * A185226  Number of disconnected 2-regular simple graphs on n vertices with
 *          girth at least 6.
 * https://oeis.org/A185226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n with each part at least 6 and
 *            at least 2 parts. Computed by dynamic programming for partitions
 *            with parts ≥ 6, then subtracting the trivial one‑part partition
 * Range:     u8 n <= 43, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *dp_cache = NULL;          /* dynamic programming table */
static idx_t  dp_cache_size = 0;        /* size of dp_cache (indexes 0..size-1) */

/* recompute the DP table up to index n (inclusive) */
static void compute_dp_up_to(idx_t n) {
    size_t newSize = (size_t)(n + 1);
    seq_t *new_cache = (seq_t*)malloc(newSize * sizeof(seq_t));
    if (!new_cache) abort();          /* allocation failure */

    /* initialise all entries to 0 and set dp[0] = 1 (empty partition) */
    memset(new_cache, 0, newSize * sizeof(seq_t));
    new_cache[0] = (seq_t)1;

    /* classic coin‑change DP: parts (coins) start from 6 */
    for (idx_t k = 6; k <= n; ++k) {
        for (idx_t i = k; i <= n; ++i) {
            new_cache[i] += new_cache[i - k];
        }
    }

    /* replace old cache (if any) */
    free(dp_cache);
    dp_cache = new_cache;
    dp_cache_size = newSize;
}

/* public entry point required by the tester */
seq_t a(idx_t n) {
    if (n < 6) return (seq_t)0;        /* no partitions with 2 parts */

    if (dp_cache_size <= (size_t)n)    /* we need a larger table */
        compute_dp_up_to(n);

    seq_t val = dp_cache[n];           /* partitions with parts ≥ 6 */
    val -= (seq_t)1;                   /* remove the single‑part partition */
    return val;
}
