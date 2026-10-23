/*
 * A180490  Numbers k such that (A000120(k))^2 divides k.
 * https://oeis.org/A180490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that (popcount(k))^2 divides k.
   a(1) = 1, a(2) = 2, a(3) = 4, ... */
/* Computes a(n) by scanning integers and storing results in a static cache. */

#include <stdint.h>
#include <stdlib.h>

/* Compute popcount of a 64‑bit integer using compiler intrinsics if available */
static inline int popcount_u64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
#endif
}

/* Return the n‑th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    /* Static cache to remember previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_k = 1;   /* next integer to test */

    if (n == 0) return 0; /* Undefined for n < 1, but guard against underflow */

    /* Ensure the cache has enough space */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure */
    }

    /* Generate terms until we have n of them */
    while (cache_len < (size_t)n) {
        int pc = popcount_u64((uint64_t)next_k);
        seq_t sq = (seq_t)pc * (seq_t)pc;

        if ((seq_t)next_k % sq == 0) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) return 0; /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
