/*
 * A013989  a(n) = (n+1)*(a(n-1)/n + a(n-2)), with a(0)=1, a(1)=2.
 * https://oeis.org/A013989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(a(n-1)/n + a(n-2)), a(0)=1, a(1)=2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 29, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static void ensure_cache(idx_t n, seq_t **cache, idx_t *cache_size)
{
    if (*cache_size <= n) {
        idx_t old_size = *cache_size;
        idx_t new_size = old_size ? (old_size << 1) : 1;
        while (new_size <= n)
            new_size <<= 1;

        void *tmp_ptr = realloc(*cache, new_size * sizeof(seq_t));
        if (!tmp_ptr)
            abort();
        *cache = (seq_t *)tmp_ptr;
        *cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = (idx_t)-1;

    if (cache == NULL) {                 /* first time we need the cache   */
        void *tmp_ptr = malloc(2 * sizeof(seq_t));
        if (!tmp_ptr)
            abort();
        cache = (seq_t *)tmp_ptr;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_size = 2;
        last_computed = 1;
    }

    ensure_cache(n, &cache, &cache_size);

    if (last_computed < n) {             /* compute any missing values   */
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t t = cache[i - 1] / (seq_t)i;
            t = t + cache[i - 2];
            cache[i] = ((seq_t)(i + 1)) * t;
        }
        last_computed = n;
    }

    return cache[n];
}
