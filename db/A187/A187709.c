/*
 * A187709  a(n) = (7*9^n + 1)/8.
 * https://oeis.org/A187709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*9^n + 1)/8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=8, a(n)=10*a(n-1)-9*a(n-2) for n≥2 */
#include <stdlib.h>

/* Cache for previously computed values */
static seq_t *cache = NULL;   /* array of seq_t holding a(0) … a(k) */
static idx_t cache_size = 0;  /* number of elements stored in cache */

/* Ensure the cache can hold indices 0 … new_n */
static void ensure_cache(idx_t new_n)
{
    if (new_n + 1 <= cache_size)
        return; /* already large enough */

    size_t new_size = (size_t)(new_n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE); /* allocation failure */

    cache = new_cache;
    cache_size = new_size;
}

/* Return the n‑th term of sequence A187709 */
seq_t a(idx_t n)
{
    /* Initialise cache with a(0) and a(1) if not already done */
    if (cache_size == 0) {
        ensure_cache(1);        /* need space for indices 0 and 1 */
        cache[0] = (seq_t)1;    /* a(0) = 1 */
        cache[1] = (seq_t)8;    /* a(1) = 8 */
        cache_size = 2;
    }

    /* Already computed */
    if (n < cache_size)
        return cache[n];

    /* Compute values from cache_size up to n */
    idx_t start = cache_size;  /* first index to compute */
    ensure_cache(n);
    for (idx_t i = start; i <= n; ++i) {
        /* a(i) = 10*a(i-1) - 9*a(i-2) */
        seq_t term1 = cache[i - 1] * (seq_t)10;
        seq_t term2 = cache[i - 2] * (seq_t)9;
        cache[i] = term1 - term2;
    }

    cache_size = n + 1;
    return cache[n];
}
