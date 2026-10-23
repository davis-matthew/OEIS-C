/*
 * A029103  Expansion of 1/((1-x)(1-x^5)(1-x^10)(1-x^11)).
 * https://oeis.org/A029103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 1, 5, 10, and 11
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t max_cached = 0;
static bool initialized = false;
static const idx_t PART_SIZES[4] = {1,5,10,11};

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Initialise cache with the base case a(0) = 1 */
        cache_size = 1;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        max_cached = 0;
        initialized = true;
    }

    /* Recompute the sequence up to n if we haven't reached it yet */
    if (n > max_cached) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_cache) abort();

        /* initialise all entries to zero */
        for (idx_t i = 0; i < new_size; ++i)
            new_cache[i] = (seq_t)0;
        new_cache[0] = (seq_t)1;  /* a(0) = 1 */

        /* dynamic‑programming build over the allowed parts */
        for (int p = 0; p < 4; ++p) {
            idx_t s = PART_SIZES[p];
            for (idx_t i = s; i < new_size; ++i) {
                new_cache[i] += new_cache[i - s];
            }
        }

        free(cache);
        cache = new_cache;
        cache_size = new_size;
        max_cached = n;
    }

    return cache[n];
}
