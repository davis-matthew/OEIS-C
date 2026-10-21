/*
 * A057092  Scaled Chebyshev U-polynomials evaluated at i*3/2. Generalized
 *          Fibonacci sequence.
 * https://oeis.org/A057092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*(a(n-1) + a(n-2)), with a(0)=1, a(1)=9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)9;

#ifdef __cplusplus
    /* Use a static std::vector to cache computed values in C++. */
    static std::vector<seq_t> cache = [] {
        std::vector<seq_t> v(2);
        v[0] = (seq_t)1;
        v[1] = (seq_t)9;
        return v;
    }();

    /* Extend the cache until it contains a(n). */
    while (cache.size() <= n) {
        seq_t prev1 = cache[cache.size() - 1];
        seq_t prev2 = cache[cache.size() - 2];
        cache.push_back((seq_t)(9 * (prev1 + prev2)));
    }
    return cache[n];
#else
    /* C implementation using a static malloc‑based array. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL) /* allocation failure: return 0 (undefined behavior otherwise) */
            return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
    }

    if (n >= cache_len) {
        /* Grow the array to hold up to a(n). */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (tmp == NULL) /* allocation failure: return 0 (undefined behavior otherwise) */
            return (seq_t)0;
        cache = tmp;
        for (idx_t i = cache_len; i < new_len; ++i) {
            cache[i] = (seq_t)(9 * (cache[i - 1] + cache[i - 2]));
        }
        cache_len = new_len;
    }

    return cache[n];
#endif
}
