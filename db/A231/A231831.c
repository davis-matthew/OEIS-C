/*
 * A231831  a(0) = 1; for n > 0, a(n) = -1 + 4*Product_{i=0..n-1} a(i)^2.
 * https://oeis.org/A231831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A231831:
   a(0) = 1
   a(1) = 3
   for n ≥ 2: a(n) = (a(n‑1) + 1) * a(n‑1)² – 1
   (equivalently: a(n) = –1 + 4·Π_{i=0}^{n‑1} a(i)²)          */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

/* ---------- C implementation ----------
   Provides a (semi) memoised version that works for any integer
   type seq_t, including arbitrary‑precision integer classes.  */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* Static vector cache, initialized with the first two terms. */
    static std::vector<seq_t> cache{ (seq_t)1, (seq_t)3 };

    if ((size_t)n < cache.size())
        return cache[(size_t)n];

    /* Expand the cache up to n and compute missing terms. */
    cache.resize((size_t)n + 1);
    for (size_t i = cache.size() - 1; i <= (size_t)n; ++i) {
        const seq_t prev = cache[i - 1];
        seq_t sq   = prev * prev;                /* a(i‑1)²          */
        seq_t prod = (prev + (seq_t)1) * sq;      /* (a+1)*a²         */
        cache[i] = prod - (seq_t)1;              /* a(i)              */
    }
    return cache[(size_t)n];
#else
    /* C implementation with a plain malloc/realloc cache. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;  /* number of stored elements   */
    static size_t cache_cap  = 0;  /* current capacity            */

    /* Initialise cache with first two terms on first call. */
    if (cache_size == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache_size = 2;
    }

    /* Return stored value if already computed. */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Ensure capacity for n+1 elements. */
    while (cache_cap <= (size_t)n) {
        cache_cap = cache_cap ? cache_cap * 2 : 1;
    }
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute missing terms from cache_size up to n. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        const seq_t prev = cache[i - 1];
        seq_t sq   = prev * prev;                /* prev²             */
        seq_t prod = (prev + (seq_t)1) * sq;      /* (prev+1)*prev²    */
        cache[i] = prod - (seq_t)1;              /* a(i)              */
    }
    cache_size = n + 1;
    return cache[(size_t)n];
#endif
}
