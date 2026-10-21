/*
 * A068962  Number of successive terms of A028356 that add to n; or length of
 *          n-th term of A028355.
 * https://oeis.org/A068962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + a(n‑15) – a(n‑16) for n > 16, with a(1)=1,
 *            a(2)=1, a(3)=1, a(4)=1, a(5)=2, a(6)=3, a(7)=2, a(8)=4, a(9)=3,
 *            a(10)=4, a(11)=5, a(12)=5, a(13)=5, a(14)=5, a(15)=6, a(16)=7.
 *            The function computes the n‑th term in O(n) time and memoises
 *            the computed values
 * Range:     u8 n <= 637, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, abort */

/* seq_t and idx_t are defined by the build system. */
static const seq_t init_vals[17] = {
    0, 1, 1, 1, 1, 2, 3, 2, 4, 3, 4, 5, 5, 5, 5, 6, 7
};

static seq_t *cache      = NULL;      /* memoised values */
static size_t cache_cap  = 0;          /* allocated size of cache */
static idx_t  last_calc  = 0;          /* largest n that has been calculated */

/* grow the cache so that index n is valid */
static void grow_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = (size_t)n + 1;   /* include index 0 */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */
    grow_cache(n);
    for (idx_t i = last_calc + 1; i <= n; ++i) {
        if (i <= 16) {
            cache[i] = init_vals[i];
        } else {
            cache[i] = cache[i-1] + cache[i-15] - cache[i-16];
        }
    }
    last_calc = n;
    return cache[n];
}
