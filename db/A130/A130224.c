/*
 * A130224  a(1) = 1; a(n) = a(n-1) + (number of times the digit 1 has
 *          appeared in the sequence so far).
 * https://oeis.org/A130224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 1; a(n) = a(n-1) + (number of times digit 1 has appeared
 *            in a(1)…a(n-1))
 * Range:     u8 n <= 41, u16 n <= 468, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_ones_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements the recurrence using a static cache so that calls in any order are handled. */

#include <stdlib.h>
#include "../utils/count_ones_seq_t.h"

/* Static cache state */
static seq_t *cache      = NULL;   /* array of computed a(n) */
static size_t   cache_cap = 0;     /* allocated capacity */
static idx_t    cache_len = 0;     /* number of terms stored (max n computed) */
static seq_t    ones_so_far = 0;   /* total count of digit '1' seen up to a(cache_len) */

/* Ensure the cache has room for at least 'need' terms */
static void ensure_cache_size(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < need)
        new_cap = need;

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* In an actual build we would report an error; here we simply abort. */
        abort();
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Main function as required by the contributor interface */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* offset is 1; undefined for n<1 */

    size_t target = (size_t)n;
    if (target > cache_len) {
        ensure_cache_size(target);

        /* initialise first term if not already done */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            ones_so_far = count_ones_seq_t(cache[0]);   /* 1 */
            cache_len = 1;
        }

        /* compute missing terms up to target */
        for (size_t i = cache_len; i < target; ++i) {
            seq_t prev       = cache[i - 1];
            seq_t new_val    = prev + ones_so_far;
            cache[i]         = new_val;
            ones_so_far      += count_ones_seq_t(new_val);
        }
        cache_len = target;
    }

    return cache[target - 1];
}
