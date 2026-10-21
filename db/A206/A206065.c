/*
 * A206065  Number of (n+1) X 2 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly three clockwise edge increases.
 * https://oeis.org/A206065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 2*a(n-4) for n>5 Initial values:
 *            a(1)=81, a(2)=150, a(3)=537, a(4)=2052, a(5)=7878 Sequence is
 *            computed by the recurrence and cached for efficient repeated
 *            calls
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of already computed terms */
static size_t cache_cap = 0;         /* current allocation size (terms+1) */
static idx_t  cache_max = 0;         /* highest n computed so far */

/* base values for n = 1..5, index 0 unused */
static const seq_t base_terms[6] = {
    (seq_t)0,
    (seq_t)81,
    (seq_t)150,
    (seq_t)537,
    (seq_t)2052,
    (seq_t)7878
};

seq_t a(idx_t n)
{
    /* handle the trivial interval */
    if (n <= 5) return base_terms[n];

    /* ensure the cache can hold values up to n */
    size_t needed_cap = (size_t)n + 1;   /* indices 0..n inclusive */
    if (!cache) {
        cache = (seq_t*)malloc(needed_cap * sizeof(seq_t));
        if (!cache) return 0;
        cache_cap = needed_cap;
        cache_max = 0;
    } else if (needed_cap > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, needed_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_cap = needed_cap;
    }

    /* fill in base terms that might still be missing */
    if (cache_max < 5) {
        for (size_t i = cache_max + 1; i <= 5; ++i)
            cache[i] = base_terms[i];
        cache_max = 5;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = 4 * cache[i - 1] + 2 * cache[i - 4] - cache[i - 2];
    }
    cache_max = n;

    return cache[n];
}
