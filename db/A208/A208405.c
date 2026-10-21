/*
 * A208405  Number of nX5 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1008*a(n-1) -28108*a(n-2) +138160*a(n-3) * -223680*a(n-4)
 *            +168192*a(n-5) -55296*a(n-6) for n > 7 * Initial values
 *            a(1)…a(7) are taken from the OEIS entry * and stored in an
 *            internal cache. The cache is grown * on demand, so calls to a(n)
 *            can be made in any order
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

/* Initial terms as strings */
static const char *const init_str[8] = {
    NULL,
    "51",
    "41914",
    "40883360",
    "40039156736",
    "39216100214432",
    "38410047476679936",
    "37620564383817645696"
};

/* Cache for computed values: indices 0 … cache_max */
static seq_t *cache      = NULL;   /* pointer to cache array        */
static idx_t cache_max   = 0;      /* maximum n whose value is stored */
static size_t cache_size = 0;      /* allocated size of cache array  */

/* Initialise the cache with the known base values. */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    /* Allocate space for indices 0 … 7 (inclusive). */
    cache_size = 8;
    cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
    if (!cache)
        exit(1);

    /* cache[0] is unused but kept zero for safety. */
    cache[0] = 0;
    for (idx_t i = 1; i <= 7; ++i)
        cache[i] = seq_from_str(init_str[i]);

    cache_max = 7;
}

/* Extend the cache up to at least n, computing any missing terms
 * using the empirical recurrence. */
static void extend_cache(idx_t n)
{
    if (n <= cache_max)
        return;

    /* Grow the array if necessary. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = cache_size;
        while ((size_t)n + 1 > new_size)
            new_size <<= 1;          /* double until it fits */

        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            exit(1);
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute new entries from the last known value up to n. */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t val = 1008 * cache[k - 1];
        val -= 28108 * cache[k - 2];
        val += 138160 * cache[k - 3];
        val -= 223680 * cache[k - 4];
        val += 168192 * cache[k - 5];
        val -= 55296 * cache[k - 6];
        cache[k] = val;
    }
    cache_max = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* unused, but harmless */

    init_cache();
    if (n > cache_max)
        extend_cache(n);

    return cache[n];
}
