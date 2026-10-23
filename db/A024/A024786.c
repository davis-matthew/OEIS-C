/*
 * A024786  Number of 2's in all partitions of n.
 * https://oeis.org/A024786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..⌊n/2⌋} P(n-2k) * where P(m) is the number of
 *            partitions of m (P(0)=1, P(m)=∑_{j≠0} (-1)^{j-1} P(m - g_j), *
 *            g_j = j(3j-1)/2)
 * Range:     u8 n <= 15, u16 n <= 38, u32 n <= 115, u64 n <= 384,
 *            u128 n <= 1378, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *partition_cache = NULL;   /* p[0..m] */
static idx_t partition_cache_size = 0;  /* size stored in partition_cache, >=0 */

/* Ensure partition numbers are computed up to n (inclusive). */
static void ensure_partitions(idx_t n)
{
    if (n < partition_cache_size)
        return;

    /* Allocate or expand the cache. */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(partition_cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – abort. */
        abort();
    }
    partition_cache = new_cache;

    /* Compute missing partition numbers. */
    if (partition_cache_size == 0) {
        /* Initialise p[0] = 1. */
        partition_cache[0] = 1;
        partition_cache_size = 1;
    }

    for (idx_t i = partition_cache_size; i <= n; ++i) {
        if (i == 0) {
            partition_cache[0] = 1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i)
                break;
            seq_t term = partition_cache[i - g1];
            if ((k & 1) == 1)
                sum += term;
            else
                sum -= term;

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > i)
                continue;
            term = partition_cache[i - g2];
            if ((k & 1) == 1)
                sum += term;
            else
                sum -= term;
        }
        partition_cache[i] = sum;
    }
    partition_cache_size = new_size;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* Not defined below offset; return 0 as safe fallback. */

    ensure_partitions(n);

    seq_t result = 0;
    for (idx_t k = 1; (k << 1) <= n; ++k) {
        result += partition_cache[n - (k << 1)];
    }
    return result;
}
