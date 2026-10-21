/*
 * A224672  Number of (n+1) X 5 0..2 matrices with each 2 X 2 subblock
 *          idempotent.
 * https://oeis.org/A224672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5) (n > 6)
 *            Initial values: a(1)=70, a(2)=123, a(3)=170, a(4)=260, a(5)=381,
 *            a(6)=573
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init[6] = {70, 123, 170, 260, 381, 573};

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cached_len = 0;
#endif

static void ensure_terms(idx_t n)
{
    if (n == 0)
        return;
#ifdef __cplusplus
    if (cache.size() >= (size_t)n)
        return;
    size_t old = cache.size();
    cache.resize(n);
    if (old < 6) {
        for (size_t i = old; i < 6 && i < n; ++i)
            cache[i] = init[i];
    }
    size_t start = (old < 6 ? 6 : old);
    for (size_t i = start; i < n; ++i) {
        cache[i] =
            cache[i - 1] * (seq_t)4
            - cache[i - 2] * (seq_t)5
            + cache[i - 3]
            + cache[i - 4] * (seq_t)2
            - cache[i - 5];
    }
#else
    if (cached_len >= (size_t)n)
        return;
    seq_t *tmp = realloc(cache, n * sizeof(seq_t));
    cache = tmp;
    if (cached_len < 6) {
        for (size_t i = cached_len; i < 6 && i < n; ++i)
            cache[i] = init[i];
    }
    size_t start = (cached_len < 6 ? 6 : cached_len);
    for (size_t i = start; i < n; ++i) {
        cache[i] =
            cache[i - 1] * (seq_t)4
            - cache[i - 2] * (seq_t)5
            + cache[i - 3]
            + cache[i - 4] * (seq_t)2
            - cache[i - 5];
    }
    cached_len = n;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset 1; undefined for n=0 */
    ensure_terms(n);
#ifdef __cplusplus
    return cache[n - 1];
#else
    return cache[n - 1];
#endif
}
