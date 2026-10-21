/*
 * A217777  Expansion of (1+x)*(1+2*x)*(1-x)/(1-5*x^2+5*x^4).
 * https://oeis.org/A217777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-2) - 5*a(n-4) * Initial values: a(0)=1, a(1)=2,
 *            a(2)=4, a(3)=8 * * The function uses an incremental cache so
 *            that each call may arrive * in any order; it grows the cache as
 *            necessary
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 34, u64 n <= 68, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* pointer to cached terms */
static idx_t cache_cap = 0;      /* current capacity of the cache */
static idx_t cache_len = 0;      /* number of terms actually stored (0..cache_len-1) */

/* Ensure the cache can hold at least `need` entries.  The cache
 * grows geometrically to keep allocations O(1). */
static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) {
        /* allocation failed – aborting is the safest option */
        abort();
    }
    cache = p;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialisation of the first four terms if required */
    if (cache_len == 0) {
        ensure_capacity(4);
        cache[0] = 1;   /* a(0) */
        cache[1] = 2;   /* a(1) */
        cache[2] = 4;   /* a(2) */
        cache[3] = 8;   /* a(3) */
        cache_len = 4;
    }

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[(size_t)n];

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (5 * cache[(size_t)(i - 2)]) - (5 * cache[(size_t)(i - 4)]);
        ensure_capacity(i + 1);
        cache[i] = val;
    }
    cache_len = n + 1;   /* all terms 0..n are now computed */

    return cache[(size_t)n];
}
