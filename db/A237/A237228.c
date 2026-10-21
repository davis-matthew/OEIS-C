/*
 * A237228  Number of (n+1) X (2+1) 0..2 arrays with the maximum plus the
 *          minimum of every 2 X 2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A237228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 5*a(n-3) + 21*a(n-4) + 12*a(n-5) +
 *            4*a(n-6) for n > 8, with initial values a(1)=268, a(2)=984,
 *            a(3)=3284, a(4)=10888, a(5)=36180, a(6)=120540, a(7)=401520,
 *            a(8)=1336540
 * Range:     u8 none, u16 n <= 5, u32 n <= 14, u64 n <= 33, u128 n <= 70,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

static const seq_t init[9] = {0,
    (seq_t)268, (seq_t)984, (seq_t)3284, (seq_t)10888,
    (seq_t)36180, (seq_t)120540, (seq_t)401520, (seq_t)1336540};

static seq_t *cache = NULL;       /* pointer to cached values */
static size_t cache_cap = 0;      /* allocated size of cache */
static idx_t max_computed = 0;    /* highest n for which cache[n] is valid */

/* Grow the cache so that it holds values up to at least n. */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= (size_t)n + 1)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)n + 1)
        new_cap <<= 1;

    seq_t *new_cache = alloc_seq(new_cap);
    if (!new_cache)
        return;               /* allocation failure – the caller will return 0 */

    /* copy any previously computed entries */
    for (idx_t i = 0; i <= max_computed && i < (idx_t)cache_cap; ++i)
        new_cache[i] = cache[i];

    free_seq(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* value for n<1 is undefined – return 0 */

    if (n <= 8)
        return init[n];

    /* make sure we have space for all indices up to n */
    ensure_cache(n);
    if (!cache)               /* allocation failed */
        return 0;

    /* Initialise base values once */
    if (max_computed < 8) {
        max_computed = 8;
        for (idx_t i = 1; i <= 8; ++i)
            cache[i] = init[i];
    }

    /* Compute missing terms */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = cache[i-1]
                     + ((seq_t)4) * cache[i-2]
                     + ((seq_t)5) * cache[i-3]
                     + ((seq_t)21) * cache[i-4]
                     + ((seq_t)12) * cache[i-5]
                     + ((seq_t)4) * cache[i-6];
        }
        max_computed = n;
    }

    return cache[n];
}
