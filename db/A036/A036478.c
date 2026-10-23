/*
 * A036478  a(n) = partition(11n+4) mod 11.
 * https://oeis.org/A036478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = partition(11*n+4) mod 11
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache of partition values modulo 11 */
static seq_t *p_cache = NULL;
static size_t p_cache_size = 0;

/* Extend the cache to at least `need` entries (0..need inclusive) */
static void ensure_partition_cache(size_t need)
{
    if (p_cache_size > need)
        return;
    size_t new_size = p_cache_size == 0 ? 16 : p_cache_size * 2;
    while (new_size <= need)
        new_size *= 2;
    seq_t *new_cache = (seq_t*)realloc(p_cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – aborting; caller will handle error */
        abort();
    }
    /* Initialize newly allocated entries to 0 */
    for (size_t i = p_cache_size; i < new_size; ++i)
        new_cache[i] = (seq_t)0;
    p_cache = new_cache;
    p_cache_size = new_size;
}

/* Compute partition numbers modulo 11 up to index `max_k` */
static void compute_partitions_upto(size_t max_k)
{
    /* Ensure cache big enough */
    ensure_partition_cache(max_k);
    /* If cache already computed up to max_k, nothing to do */
    static size_t computed_up_to = 0;
    if (computed_up_to > max_k)
        return;
    if (computed_up_to == 0) {
        p_cache[0] = (seq_t)1;          /* p(0) = 1 */
        computed_up_to = 0;
    }
    /* Compute remaining values */
    for (size_t k = computed_up_to + 1; k <= max_k; ++k) {
        int sum = 0;
        for (int j = 1; ; ++j) {
            size_t g1 = (size_t)j * (3U * j - 1) / 2;
            if (g1 > k) break;
            int sign = (j % 2 == 1) ? 1 : -1;
            sum += sign * (int)p_cache[k - g1];
            size_t g2 = (size_t)j * (3U * j + 1) / 2;
            if (g2 > k) continue;
            sum += sign * (int)p_cache[k - g2];
        }
        /* Convert sum to modulo 11 in the [0,10] range */
        int mod = sum % 11;
        if (mod < 0) mod += 11;
        p_cache[k] = (seq_t)mod;
    }
    computed_up_to = max_k;
}

/* Required function */
seq_t a(idx_t n)
{
    /* Compute k = 11*n + 4 (safe for n within test range) */
    idx_t k_idx = (idx_t)11 * n + 4;
    size_t k = (size_t)k_idx;
    compute_partitions_upto(k);
    return p_cache[k];
}
