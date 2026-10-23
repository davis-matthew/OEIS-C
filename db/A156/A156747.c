/*
 * A156747  a(1) = a(2) = a(3) = 0, then a(n) = abs(a(n-1) + 2*a(n-2) -
 *          a(n-3)) - a(n-2) - 1.
 * https://oeis.org/A156747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = abs(a(n-1) + 2*a(n-2) - a(n-3)) - a(n-2) - 1, with
 *            a(1)=a(2)=a(3)=0
 * Range:     i8 n <= 270, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif

#ifdef __cplusplus
static std::vector<seq_t> cache;

static void ensure_cache(idx_t n)
{
    if (cache.size() < n)
        cache.resize(n);          // new entries left uninitialized
    /* initialise the first three values to 0 */
    if (cache.size() >= 3) {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
    }
}
#else
#include <stdlib.h>
static seq_t *cache = NULL;
static size_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (cache_size < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) /* out of memory: abort or handle accordingly */ ;
        /* initialise newly allocated part to 0 */
        for (size_t i = cache_size; i < n; ++i) tmp[i] = 0;
        cache = tmp;
        cache_size = n;
    }
    /* initialise the first three values to 0 */
    if (cache_size >= 3) {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
    }
}
#endif

seq_t a(idx_t n)
{
    if (n <= 3) return 0;

    size_t old_size;
#ifdef __cplusplus
    old_size = cache.size();
#else
    old_size = cache_size;
#endif
    ensure_cache(n);

    idx_t start = old_size + 1;
    if (start < 4) start = 4;

    for (idx_t i = start; i <= n; ++i) {
        seq_t prev1 = cache[i - 2]; /* a(i-1) */
        seq_t prev2 = cache[i - 3]; /* a(i-2) */
        seq_t prev3 = cache[i - 4]; /* a(i-3) */

        seq_t t = prev1 + prev2 + prev2 - prev3; /* a(i-1) + 2*a(i-2) - a(i-3) */
        if (t < 0) t = -t;
        cache[i - 1] = t - prev2 - 1;            /* abs(...) - a(i-2) - 1 */
    }

    return cache[n - 1];
}
