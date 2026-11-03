/*
 * A099844  An Alexander sequence for the knot 8_2.
 * https://oeis.org/A099844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 123,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 977 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - 3*a(n-4) + 3*a(n-5) - a(n-6)  for n ≥ 6,  a(0)=1, a(1)=3, a(2)=6, a(3)=12, a(4)=24, a(5)=48 */

#include <stdlib.h>
#include "../utils/mem.h"

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

static const seq_t init_vals[6] = { 1, 3, 6, 12, 24, 48 };
static seq_t *cache = NULL;   /* dynamically expanded cache of terms */
static idx_t  cache_len = 0;   /* current number of cached terms */

/* Return the nth term of OEIS A099844 */
seq_t a(idx_t n)
{
    /* Base cases for the first six terms. */
    if (n < 6)
        return init_vals[n];

    /* If the value is already cached, return it immediately. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to accommodate index n. */
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = mem_resize(seq_t, cache, new_len);
    if (!new_cache)      /* allocation failure – unlikely but handled */
        return 0;
    cache = new_cache;

    /* If the cache was empty or partially filled, initialise the first six values. */
    if (cache_len < 6) {
        for (idx_t i = cache_len; i < 6; ++i)
            cache[i] = init_vals[i];
    }

    /* Compute new terms via the recurrence, starting from the first missing index. */
    idx_t start = (cache_len > 6) ? cache_len : 6;
    for (idx_t i = start; i <= n; ++i) {
        seq_t term = mul_small(cache[i - 1], 3)
                    - mul_small(cache[i - 2], 3)
                    + mul_small(cache[i - 3], 3)
                    - mul_small(cache[i - 4], 3)
                    + mul_small(cache[i - 5], 3)
                    - cache[i - 6];
        cache[i] = term;
    }

    cache_len = new_len;
    return cache[n];
}
