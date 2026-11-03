/*
 * A174752  x-values in the solution to x^2-41*y^2=1.
 * https://oeis.org/A174752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4098*a(n-1) - a(n-2); a(1)=1, a(2)=2049
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 197 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static inline seq_t seq_one(void) { return (seq_t)1; }

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Memoisation cache: cache[i] holds a(i) for i >= 1 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = 0;

    /* Protect against n < 1 – should never be requested */
    if (n < 1) return seq_one();

    /* Grow cache if necessary (cache indices go from 1 to n) */
    if (cache_cap < (size_t)(n + 1)) {
        cache = seq_realloc(cache, cache_cap, n + 1);
        cache_cap = n + 1;
    }

    /* Initialise base cases if not already set */
    if (cache_last < 2) {
        if (cache_cap < 3) {
            cache = seq_realloc(cache, cache_cap, 3);
            cache_cap = 3;
        }
        cache[1] = seq_one();
        cache[2] = (seq_t)2049U;
        cache_last = 2;
    }

    /* If the result is already cached, return it */
    if (n <= cache_last) return cache[n];

    /* Compute all intermediate terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t prod = mul_small(cache[i - 1], 4098u);
        cache[i]   = prod - cache[i - 2];
    }
    cache_last = n;
    return cache[n];
}
