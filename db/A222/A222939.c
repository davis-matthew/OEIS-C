/*
 * A222939  Number of n X 1 0..4 arrays with no element equal to another at a
 *          city block distance of exactly two, and new values 0..4 introduced
 *          in row major order.
 * https://oeis.org/A222939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (256 + 9*2^(3+n) + 5*4^n) / 384 for n>2 Recurrence: a(n)
 *            = 7*a(n-1) - 14*a(n-2) + 8*a(n-3) for n>5 Initial values:
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=7, a(5)=20
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__cplusplus)
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined; return 0 for safety */
#if defined(__cplusplus)
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(6);          // indices 0..5; 0 unused
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 3;
        cache[4] = 7;
        cache[5] = 20;
    }
    if (n < cache.size()) return cache[n];
    size_t oldsize = cache.size();     // current size (6 after init)
    cache.resize(n + 1);               // resize to hold indices up to n
    for (size_t i = oldsize; i <= n; ++i) {
        // i >= 6 here
        seq_t val = 7 * cache[i-1];
        val -= 14 * cache[i-2];
        val += 8 * cache[i-3];
        cache[i] = val;
    }
    return cache[n];
#else
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   // highest index stored (>=5 after init)
    if (cache == NULL) {
        cache_size = 5;
        cache = malloc((cache_size + 1) * sizeof(seq_t));
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 3;
        cache[4] = 7;
        cache[5] = 20;
    }
    if (n <= cache_size) return cache[n];

    size_t new_size = n + 1;
    seq_t *tmp = realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {   /* allocation failed */
        free(cache);
        cache = NULL;
        cache_size = 0;
        return 0;
    }
    cache = tmp;
    for (size_t i = cache_size + 1; i <= n; ++i) {
        seq_t val = 7 * cache[i-1];
        val -= 14 * cache[i-2];
        val += 8 * cache[i-3];
        cache[i] = val;
    }
    cache_size = n;
    return cache[n];
#endif
}
