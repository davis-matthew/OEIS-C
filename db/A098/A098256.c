/*
 * A098256  First differences of Chebyshev polynomials S(n,443) = A098254(n)
 *          with Diophantine property.
 * https://oeis.org/A098256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 443*a(n-1) - a(n-2); a(0) = 1, a(1) = 442
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_len = 0;

seq_t a(idx_t n)
{
    /* Handle the first two terms without caching */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)442;

    /* Initialize cache with the first two terms */
    if (!seq_cache) {
        seq_cache_len = 2;
        seq_cache = (seq_t*)malloc(seq_cache_len * sizeof(seq_t));
        if (!seq_cache) return (seq_t)0;                 /* out of memory */
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)442;
    }

    /* If the requested term is already computed, return it */
    if (n < seq_cache_len)
        return seq_cache[n];

    /* Extend the cache to accommodate up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0; /* out of memory */
    seq_cache = tmp;

    /* Compute missing terms iteratively */
    for (idx_t i = seq_cache_len; i <= n; ++i) {
        seq_t prev1 = seq_cache[i - 1];
        seq_t prev2 = seq_cache[i - 2];
        seq_t val;

        /* Use 128‑bit arithmetic for small integer types to avoid overflow */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            unsigned __int128 prod = (unsigned __int128)443 * (unsigned __int128)prev1;
            prod -= (unsigned __int128)prev2;
            val = (seq_t)prod;
        } else {
            /* For arbitrary‑precision types, rely on operator overloading */
            val = (seq_t)443 * prev1 - prev2;
        }
        seq_cache[i] = val;
    }
    seq_cache_len = new_len;
    return seq_cache[n];
}
