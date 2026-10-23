/*
 * A163207  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*(a(n-1)+a(n-2)+a(n-3) - 14*a(n-4)) for n ≥ 5 with
 *            initial values: a(0) = 1 a(1) = 29 a(2) = 812 a(3) = 22736 a(4)
 *            = 636202 (offset 0)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic cache of computed values */
static size_t cache_len = 0;  /* number of elements currently stored */
static size_t cache_cap = 0;  /* capacity of the cache array */

/* initialise the cache with the first five terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;

#if defined(__cplusplus)
    cache = new seq_t[5];
#else
    cache = (seq_t *)malloc(5 * sizeof(seq_t));
#endif
    if (cache == NULL)
        return; /* allocation failure – caller will handle if needed */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)29;
    cache[2] = (seq_t)812;
    cache[3] = (seq_t)22736;
    cache[4] = (seq_t)636202;

    cache_len = 5;
    cache_cap = 5;
}

/* resize cache to hold at least ‘new_len’ elements */
static void ensure_cache_len(size_t new_len)
{
    if (new_len <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < new_len)
        new_cap = new_len;

#if defined(__cplusplus)
    seq_t *new_cache = new seq_t[new_cap];
#else
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (new_cache == NULL)
        return; /* allocation failure – caller will handle if needed */

    /* copy existing values */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

#if defined(__cplusplus)
    delete[] cache;
#else
    free(cache);
#endif

    cache = new_cache;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;          /* safe if n fits in size_t */
    init_cache();

    if (idx < cache_len)
        return cache[idx];

    ensure_cache_len(idx + 1);
    if (cache == NULL) /* allocation failed */
        return (seq_t)0; /* fallback */

    /* compute terms up to idx (inclusive) */
    for (size_t i = cache_len; i <= idx; ++i) {
        seq_t sum = cache[i-1] + cache[i-2] + cache[i-3];
        seq_t sub = (seq_t)14 * cache[i-4];
        seq_t temp = sum - sub;
        cache[i] = (seq_t)27 * temp;
    }

    cache_len = idx + 1;
    return cache[idx];
}
