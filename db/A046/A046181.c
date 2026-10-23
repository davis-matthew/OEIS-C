/*
 * A046181  Indices of octagonal numbers which are also triangular.
 * https://oeis.org/A046181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 98*a(n-2) - a(n-4) - 32; Initial values: a(1)=1, a(2)=3,
 *            a(3)=63, a(4)=261
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Persistent cache to allow arbitrary-order calls */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure the cache contains values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    /* Grow the cache */
    idx_t new_len = n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed; terminate program (cannot recover) */
        exit(1);
    }
    cache = new_cache;

    /* Compute new terms */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)           val = 1;
        else if (i == 2)      val = 3;
        else if (i == 3)      val = 63;
        else if (i == 4)      val = 261;
        else {
            /* a(i) = 98*a(i-2) - a(i-4) - 32   (i >= 5) */
            seq_t a_i_2 = cache[i - 3];   /* a(i-2) stored at index (i-2)-1 = i-3 */
            seq_t a_i_4 = cache[i - 5];   /* a(i-4) stored at index (i-4)-1 = i-5 */
            val = 98 * a_i_2 - a_i_4 - 32;
        }
        cache[i - 1] = val;
    }
    cache_len = n;
}

/* Return the n-th term of A046181 (1‑based index) */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n - 1];
}
