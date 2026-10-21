/*
 * A099139  a(n) = (18^n - (-6)^n)/24.
 * https://oeis.org/A099139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 12*a(n-1) + 108*a(n-2), a(0) = 0, a(1) = 1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Static cache to allow arbitrary call order and O(n) time per maximum n seen. */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;

    /* Initialise the cache if needed. */
    if (cache == NULL) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_sz = 2;
    }

    /* If we already have the requested value, return it. */
    if ((size_t)n < cache_sz) return cache[n];

    /* Grow the cache to accommodate up to n. */
    size_t new_sz = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failed – in a competition setting we could exit or abort. */
        abort();
    }
    cache = tmp;

    /* Compute new entries with the recurrence. */
#if defined(__INT128__) || defined(__GNUC__) || defined(__clang__)
    /* For built‑in integer types (≤64 bits) use 128‑bit arithmetic to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        for (size_t i = cache_sz; i <= (size_t)n; ++i) {
            const seq_t a1 = cache[i-1];
            const seq_t a2 = cache[i-2];
            __int128 tmp1 = (__int128)a1 * 12;
            __int128 tmp2 = (__int128)a2 * 108;
            __int128 sum  = tmp1 + tmp2;
            cache[i] = (seq_t)sum;
        }
    } else
#endif
    {
        /* For arbitrary‑precision types use normal arithmetic operators. */
        for (size_t i = cache_sz; i <= (size_t)n; ++i) {
            const seq_t a1 = cache[i-1];
            const seq_t a2 = cache[i-2];
            cache[i] = a1 * (seq_t)12 + a2 * (seq_t)108;
        }
    }

    cache_sz = new_sz;
    return cache[(size_t)n];
}
