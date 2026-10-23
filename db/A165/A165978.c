/*
 * A165978  Largest prime factor of number formed from a(n-1) with a 1 added
 *          at the end, a(1)=2.
 * https://oeis.org/A165978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = largest_prime_factor(a(n-1) * 10 + 1), a(1) = 2
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 2 */
/* a(2) = largest_prime_factor(2*10+1) = 7 */
/* ... etc. */

#include <stdlib.h>
#include <stdint.h>

/* Largest prime factor of an unsigned 64‑bit integer. */
static uint64_t largest_prime_factor_uint64(uint64_t n)
{
    if (n < 2)
        return n;

    uint64_t max_f = 1;
    while ((n & 1ULL) == 0ULL) {   /* factor 2 */
        max_f = 2ULL;
        n >>= 1;
    }

    for (uint64_t d = 3ULL; d <= n / d; d += 2ULL) {
        while (n % d == 0ULL) {
            max_f = d;
            n /= d;
        }
    }

    if (n > 1ULL)
        max_f = n;

    return max_f;
}

/* Memoised computation of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* out of range, but safe */

    /* Static cache: cache[i] holds a(i+1). */
    static seq_t *cache   = NULL;
    static size_t cache_cap   = 0;
    static size_t cache_len   = 0;   /* current number of cached terms */

    /* Initialise cache with a(1) = 2. */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)2U;
        cache_len = 1;
    }

    /* Grow cache if necessary. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    while (cache_len < (size_t)n) {
        /* previous value a(n-1) is cache[cache_len-1] */
        uint64_t prev_val = (uint64_t)cache[cache_len - 1];
        uint64_t next_num = prev_val * 10ULL + 1ULL;
        uint64_t lpf         = largest_prime_factor_uint64(next_num);
        cache[cache_len++] = (seq_t)lpf;
    }

    return cache[n - 1];
}
