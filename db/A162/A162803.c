/*
 * A162803  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=17, a(2)=272, a(3)=4216,
   a(n)=15*a(n-1)+15*a(n-2)-120*a(n-3) for n>=4
   (derived from the rational generating function) */
#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed values */
static idx_t cache_cap = 0;   /* capacity of cache */
static idx_t cache_len = 0;   /* number of values stored (>=1 if any) */

/* Ensure the cache has room for at least 'need' entries */
static void ensure_cache_capacity(idx_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/* Compute a(n) with memoisation */
seq_t a(idx_t n) {
    /* Grow the cache so that it can store index n */
    ensure_cache_capacity(n + 1);

    /* Initialise the first four values once */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)17;
        cache[2] = (seq_t)272;
        cache[3] = (seq_t)4216;
        cache_len = 4;
    }

    /* If we already have the requested value, return it */
    if (n < cache_len) return cache[n];

    /* Compute successive terms up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t a1 = cache[k - 1];
        seq_t a2 = cache[k - 2];
        seq_t a3 = cache[k - 3];
        cache[k] = (seq_t)15 * a1 + (seq_t)15 * a2 - (seq_t)120 * a3;
    }
    cache_len = n + 1;
    return cache[n];
}
