/*
 * A122994  a(n) = a(n-1)+9*a(n-2) initialized with a(0)=1, a(1)=3.
 * https://oeis.org/A122994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 9*a(n-2); a(0) = 1; a(1) = 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cache for memoisation. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t cache_len = 0;    /* number of terms stored (0 … cache_len-1) */

static void ensure_cache_capacity(idx_t n)
{
    if (cache_cap == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }
    if (n < (idx_t)cache_cap) return;

    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Handle the two initial terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Initialise cache on first use. */
    if (cache_len == 0) {
        ensure_cache_capacity(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    /* Return cached value if already computed. */
    if (n < cache_len) return cache[n];

    /* Compute terms up to n, storing them in the cache. */
    ensure_cache_capacity(n);
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] + ((seq_t)9 * cache[i-2]);
    }
    cache_len = n + 1;   /* we now have terms 0 … n stored */
    return cache[n];
}
