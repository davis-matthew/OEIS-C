/*
 * A117791  Expansion of 1/(1 - x - x^2 + x^4 - x^6).
 * https://oeis.org/A117791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-4) + a(n-6); Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=6
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 55, u64 n <= 109,
 *            u128 n <= 218, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <cstddef>
#include <vector>
#else
#include <stddef.h>
#include <stdlib.h>
#endif

/* Storage for computed terms. */
#ifdef __cplusplus
static std::vector<seq_t> cache_;
#else
static seq_t *cache = NULL;
static size_t cache_size = 0;
#endif

/* Extend the cache to include all indices up to n inclusive. */
static void ensure_cache(idx_t n)
{
#ifdef __cplusplus
    /* C++ version using std::vector */
    if (cache_.size() > n) return;
    const size_t old_size = cache_.size();
    cache_.resize((size_t)n + 1);              /* default constructed entries */

    for (idx_t i = old_size; i <= n; ++i) {
        if (i < 6) {                         /* base values */
            switch (i) {
            case 0: cache_[0] = (seq_t)1; break;
            case 1: cache_[1] = (seq_t)1; break;
            case 2: cache_[2] = (seq_t)2; break;
            case 3: cache_[3] = (seq_t)3; break;
            case 4: cache_[4] = (seq_t)4; break;
            case 5: cache_[5] = (seq_t)6; break;
            }
        } else {                              /* recurrence */
            cache_[i] = cache_[i-1] + cache_[i-2]
                        - cache_[i-4] + cache_[i-6];
        }
    }
#else
    /* C version using malloc/realloc */
    if (cache_size > n) return;
    const size_t new_size = (size_t)n + 1;
    seq_t *new_cache = realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();                 /* critical failure */

    const size_t old_size = cache_size;
    cache = new_cache;
    cache_size = new_size;

    for (idx_t i = old_size; i <= n; ++i) {
        if (i < 6) {                         /* base values */
            switch (i) {
            case 0: cache[0] = (seq_t)1; break;
            case 1: cache[1] = (seq_t)1; break;
            case 2: cache[2] = (seq_t)2; break;
            case 3: cache[3] = (seq_t)3; break;
            case 4: cache[4] = (seq_t)4; break;
            case 5: cache[5] = (seq_t)6; break;
            }
        } else {                              /* recurrence */
            cache[i] = cache[i-1] + cache[i-2]
                        - cache[i-4] + cache[i-6];
        }
    }
#endif
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    ensure_cache(n);
#ifdef __cplusplus
    return cache_[n];
#else
    return cache[n];
#endif
}
