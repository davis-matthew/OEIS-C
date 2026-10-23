/*
 * A145182  Continued cotangent recurrence a(n+1)=a(n)^3+3*a(n) and a(1)=8.
 * https://oeis.org/A145182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued cotangent recurrence
 * a(n+1) = a(n)^3 + 3*a(n),  a(1) = 8
 * Indexed so that a(1) is the first element (OEIS offset 1). */

#include <stdlib.h>

/* Dynamic cache for already computed terms.  cache[0] == a(1). */
static seq_t *cache      = NULL;  /* pointer to array of terms */
static size_t cache_len  = 0;      /* number of terms stored so far   */
static size_t cache_cap  = 0;      /* current allocation size         */

/* Helper: ensure the cache can hold at least `min_cap` elements.
 * If the cache is too small it is grown (doubling strategy). */
static void ensure_cache_capacity(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1U : 16;
    while (new_cap < min_cap)
        new_cap <<= 1U;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)          /* defined only for n >= 1 – 0 is unused */
        return (seq_t)0;

    /* First call: initialise cache with a(1). */
    if (cache == NULL) {
        ensure_cache_capacity(1);
        cache[0] = (seq_t)8;   /* a(1) = 8 */
        cache_len = 1;
    }

    /* Return the value if already computed. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Extend cache until term `n` is available. */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        const seq_t prev = cache[i - 1];
        seq_t tmp = prev * prev * prev + (seq_t)3 * prev;
        cache[i] = tmp;
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
