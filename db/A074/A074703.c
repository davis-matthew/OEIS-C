/*
 * A074703  a(n) = n^2*a(n-1)+1, a(1)=0.
 * https://oeis.org/A074703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2*a(n-1)+1 , a(1)=0
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A straightforward recursive definition that is memoised for
   arbitrary‑precision or fixed‑width integer builds.  The function
   keeps a static array of already computed values, grows it when
   required, and fills new entries in O(n) time.  The code uses only
   operations allowed on seq_t ( + - * / % ), casting the index to
   seq_t as required. */
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;   /* cache[i] stores a(i+1) */
static size_t cache_cap = 0;  /* capacity of cache */
static size_t cache_len = 0;  /* how many terms are currently computed */

static void grow_cache(size_t needed)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);          /* memory allocation failed */
    if (cache)
    {
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n=0, return 0 */

    if ((size_t)n > cache_cap) grow_cache((size_t)n);

    /* Compute missing terms up to n */
    if (cache_len < (size_t)n)
    {
        /* a(1) is 0 */
        if (cache_len == 0) cache[0] = (seq_t)0, cache_len = 1;

        for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i)
        {
            if (i == 1)
            {
                cache[0] = (seq_t)0;
            }
            else
            {
                seq_t mult = (seq_t)i * (seq_t)i;   /* i^2 */
                cache[(size_t)(i - 1)] = mult * cache[(size_t)(i - 2)] + (seq_t)1;
            }
            ++cache_len;
        }
    }

    return cache[(size_t)(n - 1)];
}
