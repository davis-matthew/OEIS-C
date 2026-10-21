/*
 * A087839  a(n) = a(a(a(a(a(n-2))))) + a(n - a(n-2)).
 * https://oeis.org/A087839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(a(a(a(n-2))))) + a(n - a(n-2)) initial values: a(1) =
 *            1, a(2) = 1
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed values, 1‑based */
static idx_t cache_cap = 0;      /* size of allocated array (number of elements) */
static idx_t computed = 0;       /* highest index that has been computed */

seq_t
a(idx_t n)
{
    /* ensure the cache can hold indices up to n */
    if (cache == NULL) {
        idx_t cap = (n + 1 > 3) ? n + 1 : 3;    /* at least 3 for a(1), a(2) */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        cache_cap = cap;
        cache[1] = 1;
        cache[2] = 1;
        computed = 2;
    } else if (n + 1 > cache_cap) {
        idx_t cap = n + 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        cache_cap = cap;
    }

    if (n <= computed) {
        return cache[n];
    }

    for (idx_t i = computed + 1; i <= n; ++i) {
        /* second term: a(n - a(n-2)) */
        idx_t idx2 = i - (idx_t)cache[i - 2];
        seq_t second = cache[idx2];

        /* nested term: a(a(a(a(a(i-2))))) */
        idx_t x = (idx_t)cache[i - 2];
        idx_t y = (idx_t)cache[x];
        idx_t z = (idx_t)cache[y];
        idx_t w = (idx_t)cache[z];
        idx_t u = (idx_t)cache[w];
        seq_t nested = cache[u];

        cache[i] = nested + second;
    }

    computed = n;
    return cache[n];
}
