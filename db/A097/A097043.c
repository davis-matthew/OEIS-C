/*
 * A097043  a(n) = n - a(floor(sqrt(n))) for n > 1; a(1) = 1.
 * https://oeis.org/A097043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - a(floor(sqrt(n))), with a(1) = 1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Portable includes for C and C++ */
#ifdef __cplusplus
#include <vector>
#include <cmath>
#include <cstdint>
#else
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#endif

/* Integer square root for idx_t (uint64_t) */
static inline idx_t isqrt(idx_t x)
{
    if (x == 0)
        return 0;
    /* First estimate with floating point */
    idx_t r = (idx_t)sqrtl((long double)x);
    /* Correct the estimate */
    while (( (__uint128_t)(r + 1) * (r + 1)) <= x)
        ++r;
    while (( (__uint128_t)r * r) > x)
        --r;
    return r;
}

#ifdef __cplusplus
/* C++ implementation using std::vector */
static std::vector<seq_t> cache_vec;   /* cache_vec[0] unused */
static bool cache_init = false;

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if (!cache_init) {
        cache_vec.resize(2);      /* indices 0 and 1 */
        cache_vec[1] = (seq_t)1;
        cache_init = true;
    }

    if (n >= (idx_t)cache_vec.size()) {
        /* Extend the vector to needed size */
        idx_t old_size = cache_vec.size();
        cache_vec.resize(n + 1);     /* indices up to n inclusive */
        for (idx_t i = old_size; i <= n; ++i) {
            idx_t sq = isqrt(i);
            cache_vec[i] = (seq_t)i - cache_vec[sq];
        }
    }

    return cache_vec[n];
}

#else
/* C implementation using malloc/realloc */
static seq_t *cache = NULL;    /* cache[0] unused */
static idx_t cache_cap = 0;    /* allocated capacity (>= needed) */
static idx_t cache_len = 0;    /* highest index computed */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if (cache == NULL) {
        /* Initial allocation (powers of two for amortised cost) */
        cache_cap = 4;                 /* at least 4 to store indices 0..4 */
        cache = (seq_t*)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* out‑of‑memory – unlikely in tests */
        cache_len = 1;
        cache[1] = (seq_t)1;
    }

    if (cache_len < n) {
        /* Grow capacity if necessary */
        idx_t new_cap = cache_cap;
        while (new_cap < n + 1)
            new_cap <<= 1;
        cache = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;
        cache_cap = new_cap;

        /* Compute new entries */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            idx_t sq = isqrt(i);
            cache[i] = (seq_t)i - cache[sq];
        }
        cache_len = n;
    }

    return cache[n];
}
#endif
