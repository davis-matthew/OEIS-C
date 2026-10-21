/*
 * A082840  a(n) = 4*a(n-1) - a(n-2) + 3, with a(0) = -1, a(1) = 1.
 * https://oeis.org/A082840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 3, a(0) = -1, a(1) = 1
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 16, i64 n <= 33, i128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>

/* C++17 build – use std::vector for proper copy/move semantics */
static std::vector<seq_t> cache;
static bool cache_init = false;

seq_t a(idx_t n)
{
    if (!cache_init) {
        cache.resize(2);
        cache[0] = -1;
        cache[1] =  1;
        cache_init = true;
    }

    if (n < cache.size())
        return cache[n];

    size_t old = cache.size();           /* first index that needs computing */
    cache.resize(n + 1);                 /* ensure space up to n */

    for (size_t i = old; i <= n; ++i)
        cache[i] = 4 * cache[i-1] - cache[i-2] + 3;

    return cache[n];
}

#else
#include <stdlib.h>

/* C build – use malloc/realloc (seq_t is a trivial type) */
static seq_t *cache = NULL;
static idx_t cache_used = 0;   /* number of elements stored */
static idx_t cache_cap  = 0;   /* allocated capacity */

static void enlarge(idx_t need)
{
    if (cache_cap == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = -1;
        cache[1] =  1;
        cache_used = 2;
    }

    if (need < cache_used)
        return;

    if (need >= cache_cap) {
        idx_t new_cap = cache_cap * 2;
        while (new_cap <= need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – return 0 as sentinel */
            cache = NULL;
            cache_cap = cache_used = 0;
            return;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_used; i <= need; ++i)
        cache[i] = 4 * cache[i-1] - cache[i-2] + 3;

    cache_used = need + 1;
}

seq_t a(idx_t n)
{
    if (cache == NULL || n >= cache_used)
        enlarge(n);
    return cache[n];
}

#endif
