/*
 * A182510  a(0)=0, a(1)=1, a(n)=(a(n-1) XOR n) - a(n-2).
 * https://oeis.org/A182510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = (a(n-1) XOR n) - a(n-2) for n ≥ 2
 *            (XOR is the bitwise exclusive‑or operator)
 * Range:     i8 n <= 57, i16 n <= 664, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* Ensure that at least 'needed' elements can be stored. */
static inline void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap * 2 : 4;
    while (newcap < needed)
        newcap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * newcap);
    if (!tmp)
        abort();
    cache     = tmp;
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    /* Handle base cases. */
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    /* Initialise cache with the two known values once. */
    if (cache_len == 0) {
        ensure_cache_capacity(2);
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    }

    /* Return a previously computed value if available. */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Compute missing terms up to n. */
    ensure_cache_capacity(n + 1);
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (cache[i - 1] ^ (seq_t)i) - cache[i - 2];
    }
    cache_len = n + 1;
    return cache[n];
}
