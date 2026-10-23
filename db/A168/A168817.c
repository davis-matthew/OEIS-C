/*
 * A168817  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 18: a(n) = 2 + 42·∑_{k=0}^{n-1} a(k) For n
 *            = 19: a(19) = 1 + 42·∑_{k=1}^{18} a(k) – 903·a(0) For n ≥ 20:
 *            a(n) = 42·∑_{j=1}^{18} a(n-j) – 903·a(n-19)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector (works for integral and arbitrary‑precision types) */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(20);                     /* indices 0 … 19 */
        cache[0] = (seq_t)1;
        seq_t sum = cache[0];
        for (idx_t i = 1; i <= 18; ++i) {
            cache[i] = (seq_t)2 + (seq_t)42 * sum;
            sum += cache[i];
        }
        seq_t sum18 = (seq_t)0;
        for (idx_t k = 1; k <= 18; ++k) sum18 += cache[k];
        cache[19] = (seq_t)1 + (seq_t)42 * sum18 - (seq_t)903 * cache[0];
    }

    if (n < (idx_t)cache.size())
        return cache[n];

    size_t old_size = cache.size();
    cache.resize(n + 1);
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t s18 = (seq_t)0;
        for (idx_t j = 1; j <= 18; ++j)
            s18 += cache[i - j];
        cache[i] = (seq_t)42 * s18 - (seq_t)903 * cache[i - 19];
    }
    return cache[n];
#else
    /* C implementation using malloc/realloc (works for byte‑sized integral types) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (cache == NULL) {
        cache_len = 20;                        /* indices 0 … 19 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure */
        cache[0] = (seq_t)1;
        seq_t sum = cache[0];
        for (idx_t i = 1; i <= 18; ++i) {
            cache[i] = (seq_t)2 + (seq_t)42 * sum;
            sum += cache[i];
        }
        seq_t sum18 = (seq_t)0;
        for (idx_t k = 1; k <= 18; ++k) sum18 += cache[k];
        cache[19] = (seq_t)1 + (seq_t)42 * sum18 - (seq_t)903 * cache[0];
    }

    if (n < (idx_t)cache_len)
        return cache[n];

    size_t needed = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) return 0;                  /* allocation failure */
    cache = tmp;

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t s18 = (seq_t)0;
        for (idx_t j = 1; j <= 18; ++j)
            s18 += cache[i - j];
        cache[i] = (seq_t)42 * s18 - (seq_t)903 * cache[i - 19];
    }
    cache_len = needed;
    return cache[n];
#endif
}
