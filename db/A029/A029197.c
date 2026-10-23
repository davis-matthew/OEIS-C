/*
 * A029197  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^6)*(1-x^11)).
 * https://oeis.org/A029197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into the parts 2, 5, 6, and 11
 *            (computed by standard coin‑change dynamic programming)
 * Range:     u8 n <= 88, u16 n <= 625, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache of the table values; cached_len is the number of
   elements currently stored. */
static seq_t *cache = NULL;
static size_t cached_len = 0;

/* Ensure that 'cache' has space for indices 0..n (inclusive) and that
   all entries up to n are correctly computed.  The table is built
   using the coin‑change DP for parts {2,5,6,11}. */
static void ensure_cache_size(idx_t n)
{
    size_t new_len = (size_t)n + 1;
    if (!cache || new_len > cached_len) {
        /* Allocate new buffer and initialise to zero. */
        seq_t *tmp;
#ifdef __cplusplus
        tmp = new seq_t[new_len];
#else
        tmp = (seq_t *)malloc(sizeof(seq_t) * new_len);
#endif
        if (!tmp) { /* allocation failure – leave it unmodified */
            return;
        }
        for (size_t i = 0; i < new_len; ++i) {
            tmp[i] = 0;
        }

        /* Build the DP table for partitions into {2,5,6,11}. */
        tmp[0] = 1;
        const idx_t coins[4] = {2, 5, 6, 11};
        for (int j = 0; j < 4; ++j) {
            idx_t c = coins[j];
            for (idx_t i = c; i <= n; ++i) {
                tmp[i] = tmp[i] + tmp[i - c];
            }
        }

        /* Replace old cache. */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = tmp;
        cached_len = new_len;
    }
}

/* Return a(n) – number of partitions of n into 2,5,6,11. */
seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return cache[n];
}
