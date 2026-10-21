/*
 * A051047  For n > 5, a(n) = 15*a(n-1) - 15*a(n-2) + a(n-3); initial terms
 *          are 1, 3, 8, 120, 1680.
 * https://oeis.org/A051047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 15*a(n-1) - 15*a(n-2) + a(n-3) for n > 5 Initial values
 *            (n = 1..5): a(1)=1, a(2)=3, a(3)=8, a(4)=120, a(5)=1680
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache[0] stores a(1) */
static size_t cache_size = 0;        /* number of terms currently stored */
static size_t cache_cap   = 0;        /* allocated capacity */

/* Grow the cache to at least 'needed' terms */
static void cache_grow(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute term at position idx (0‑based).  idx >= 5 for this function. */
static seq_t compute_term(size_t idx)
{
    static const seq_t fifteen = (seq_t)15;
    return fifteen * cache[idx-1] - fifteen * cache[idx-2] + cache[idx-3];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1, return 0 */

    size_t idx = (size_t)n - 1;     /* convert to 0‑based index */

    /* Return known initial terms directly */
    if (idx < 5) {
        static const seq_t init[5] = { (seq_t)1, (seq_t)3, (seq_t)8,
                                       (seq_t)120, (seq_t)1680 };
        return init[idx];
    }

    /* Ensure cache can hold the requested term */
    if (cache_size <= idx) {
        cache_grow(idx + 1);

        /* Initialize first five terms if not yet done */
        if (cache_size < 5) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)3;
            cache[2] = (seq_t)8;
            cache[3] = (seq_t)120;
            cache[4] = (seq_t)1680;
            cache_size = 5;
        }

        /* Compute missing terms up to idx */
        for (size_t i = cache_size; i <= idx; ++i) {
            cache[i] = compute_term(i);
        }
        cache_size = idx + 1;
    }

    return cache[idx];
}
