/*
 * A151703  a(0)=1, a(1)=0; a(2^i+j) = a(j) + 2*a(j+1) for 0 <= j < 2^i.
 * https://oeis.org/A151703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0; a(2^i+j)=a(j)+2*a(j+1) for 0<=j<2^i
 * Range:     u8 n <= 122, u16 n <= 8179, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute terms recursively by scanning increasing indices. */
#include <stdlib.h>
#include <stddef.h>

/* Memoised cache for previously computed values. */
static seq_t *cache = NULL;          /* Array storing computed a(n) values. */
static size_t cache_len = 0;         /* Current allocated length. */
static idx_t last_computed = 0;      /* Highest index that has been computed. */

/* Helper to grow the cache to at least new_len entries. */
static void grow_cache(size_t new_len)
{
    if (cache_len >= new_len) return;
    size_t new_cap = cache_len ? cache_len * 2 : 8;
    if (new_cap < new_len) new_cap = new_len;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_len = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initialise basic terms on first call. */
    if (!cache) {
        grow_cache(2);          /* Allocate space for a(0) and a(1). */
        cache[0] = 1;
        cache[1] = 0;
        last_computed = 1;
    }

    /* If the desired term is already available, return it immediately. */
    if (n <= last_computed) return cache[n];

    /* Ensure sufficient capacity for new terms. */
    grow_cache(n + 1);

    /* Compute terms sequentially from the next index up to n. */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        /* Find the largest power of two <= k. */
        idx_t pow = 1;
        while ((pow << 1) <= k) pow <<= 1;

        idx_t j = k - pow;                /* 0 <= j < pow */
        cache[k] = cache[j] + (cache[j + 1] << 1); /* a(j) + 2*a(j+1) */
    }

    last_computed = n;
    return cache[n];
}
