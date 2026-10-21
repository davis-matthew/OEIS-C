/*
 * A080833  E.g.f.: exp( x/(1 - x - x^2) ).
 * https://oeis.org/A080833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n-1)*a(n-1) + (n-2)*(n-1)*a(n-2) -
 *            (n-2)*(n-1)*(2*n-7)*a(n-3) - (n-4)*(n-3)*(n-2)*(n-1)*a(n-4),
 *            with a(0)=1, a(1)=1, a(2)=3, a(3)=19
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static idx_t cache_len  = 0;   /* number of stored terms   */
static idx_t cache_cap  = 0;   /* current allocation size */
#endif

/* initialise first four terms */
static void
init_cache(void)
{
#ifdef __cplusplus
    if (cache.size() >= 4) return;
    cache.resize(4);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)19;
#else
    if (cache_len >= 4) return;
    if (cache) free(cache);       /* just in case */
    cache_cap = 4;
    cache = malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)19;
    cache_len = 4;
#endif
}

/* compute a(n) */
seq_t
a(idx_t n)
{
    init_cache();

#ifdef __cplusplus
    if (n < cache.size())
        return cache[(size_t)n];

    size_t old = cache.size();
    cache.resize((size_t)n + 1);

    for (size_t i = old; i <= (size_t)n; ++i) {
        if (i < 4) {
            /* already initialized, skip */
            continue;
        }
        seq_t term;
        seq_t t1 = (seq_t)(2 * i - 1) * cache[i - 1];
        seq_t t2 = (seq_t)(i - 2) * (seq_t)(i - 1) * cache[i - 2];
        seq_t t3 = (seq_t)(i - 2) * (seq_t)(i - 1) *
                   (seq_t)(2 * i - 7) * cache[i - 3];
        seq_t t4 = (seq_t)(i - 4) * (seq_t)(i - 3) *
                   (seq_t)(i - 2) * (seq_t)(i - 1) * cache[i - 4];
        term = t1 + t2 - t3 - t4;
        cache[i] = term;
    }

    return cache[(size_t)n];
#else
    if (n < cache_len)
        return cache[(size_t)n];

    size_t new_cap = (size_t)n + 1;
    if (new_cap > cache_cap) {
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 4) {
            /* already initialized, skip */
            continue;
        }
        seq_t term;
        seq_t t1 = (seq_t)(2 * i - 1) * cache[i - 1];
        seq_t t2 = (seq_t)(i - 2) * (seq_t)(i - 1) * cache[i - 2];
        seq_t t3 = (seq_t)(i - 2) * (seq_t)(i - 1) *
                   (seq_t)(2 * i - 7) * cache[i - 3];
        seq_t t4 = (seq_t)(i - 4) * (seq_t)(i - 3) *
                   (seq_t)(i - 2) * (seq_t)(i - 1) * cache[i - 4];
        term = t1 + t2 - t3 - t4;
        cache[i] = term;
    }

    cache_len = n + 1;
    return cache[(size_t)n];
#endif
}
