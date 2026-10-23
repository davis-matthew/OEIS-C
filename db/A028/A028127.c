/*
 * A028127  Expansion of 1/((1-4x)(1-5x)(1-10x)(1-11x)).
 * https://oeis.org/A028127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(0) = 1, a(1) = 30, a(2) = 581, a(3) = 9270
 *   a(n) = 30*a(n-1) - 319*a(n-2) + 1410*a(n-3) - 2200*a(n-4)  , n ≥ 4
 */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;       // memoised values
static size_t cache_cap = 0;      // allocated capacity in elements
static size_t cache_len = 0;      // number of computed values (highest index + 1)

/* Ensure cache contains values up to index n, computing any missing ones. */
static void ensure_cache(idx_t n)
{
    /* enlarge the cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = alloc_seq_array(new_cap);
        /* copy old values */
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        if (cache) free_seq_array(cache);
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0)          val = (seq_t)1;
        else if (i == 1)     val = (seq_t)30;
        else if (i == 2)     val = (seq_t)581;
        else if (i == 3)     val = (seq_t)9270;
        else {
            val = 30 * cache[i-1];
            val -= 319 * cache[i-2];
            val += 1410 * cache[i-3];
            val -= 2200 * cache[i-4];
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
