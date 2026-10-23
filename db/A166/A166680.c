/*
 * A166680  a(n) = n*(a(n-1)-4), a(0) = 1.
 * https://oeis.org/A166680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n*(a(n-1)-4), a(0) = 1
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation with memoisation. */

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> __cache;
static void __ensure_cache(idx_t n)
{
    // Expand cache up to index n (inclusive).
    if (n + 1 <= __cache.size()) return;
    __cache.resize(n + 1);
    // Compute missing terms.
    for (idx_t i = (idx_t)__cache.size() - n - 1; i <= n; ++i) {
        if (i == 0) {
            __cache[0] = (seq_t)1;
        } else {
            __cache[i] = (seq_t)i * (__cache[i - 1] - (seq_t)4);
        }
    }
}
#else
#include <stdlib.h>
static seq_t *__cache = NULL;
static idx_t __cache_len = 0;   /* number of elements stored (highest index + 1) */
static void __ensure_cache(idx_t n)
{
    if (n + 1 <= __cache_len) return;
    // Reallocate the array to hold indices up to n.
    seq_t *tmp = realloc(__cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) return;               /* allocation failed – leave cache as is */
    __cache = tmp;
    // Compute missing terms.
    for (idx_t i = __cache_len; i <= n; ++i) {
        if (i == 0) {
            __cache[0] = (seq_t)1;
        } else {
            __cache[i] = (seq_t)i * (__cache[i - 1] - (seq_t)4);
        }
    }
    __cache_len = n + 1;
}
#endif

seq_t a(idx_t n)
{
    __ensure_cache(n);
#ifdef __cplusplus
    return __cache[(size_t)n];
#else
    return __cache[n];
#endif
}
