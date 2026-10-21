/*
 * A053581  First differences of the Poly-Bernoulli numbers B_n^(k) with k=-2
 *          (A027649).
 * https://oeis.org/A053581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + C(2,2-n) (n>1), a(0) = 1, a(1) = 3.
 *            C(2,2-n) = 1 for n=2, 0 for n>2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A053581. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already-computed terms */
    static seq_t *cache = NULL;   /* caching a(k) for k <= cache_len-1 */
    static size_t cache_len = 0;  /* number of elements stored */

    /* Handle the two base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Initialise cache if not done yet */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
    }

    /* Grow the cache to include term n */
    while (cache_len <= (size_t)n) {
        seq_t val;
        if (cache_len == 2) {   /* first time computing a(2) – special term */
            val = (seq_t)5 * cache[1] - (seq_t)6 * cache[0] + (seq_t)1;
        } else {
            val = (seq_t)5 * cache[cache_len-1] - (seq_t)6 * cache[cache_len-2];
        }
        seq_t *new_cache = (seq_t*)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        /* If realloc fails we terminate silently; caller will just get an UB error. */
        if (!new_cache) break;
        cache = new_cache;
        cache[cache_len] = val;
        cache_len++;
    }

    return cache[n];
}
