/*
 * A174811  A (100,-196) Somos 4 sequence.
 * https://oeis.org/A174811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (100*a(n‑1)*a(n‑3) - 196*a(n‑2)^2)/a(n‑4), n > 3 Initial
 *            values: a(0) = 1, a(1) = 3, a(2) = 26, a(3) = 836
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper to compute a single term.
   Uses 128‑bit intermediate arithmetic when seq_t is an
   unsigned integer type of up to 64 bits; otherwise
   relies on seq_t's own operators (suitable for arbitrary‑precision). */
static seq_t compute_next(idx_t i, seq_t *cache) {
#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
    /* When seq_t is an unsigned integer type <= 64 bits,
       the following will compile and use __int128 for safety. */
    if (sizeof(seq_t) <= 8) {
        __int128 num = (__int128)100 * (__int128)cache[i-1] * (__int128)cache[i-3];
        num -= (__int128)196 * (__int128)cache[i-2] * (__int128)cache[i-2];
        return (seq_t)(num / (__int128)cache[i-4]);
    }
#endif
    /* Fallback (e.g., arbitrary‑precision seq_t). */
    seq_t num = (seq_t)100 * cache[i-1] * cache[i-3];
    num -= (seq_t)196 * cache[i-2] * cache[i-2];
    return num / cache[i-4];
}

seq_t a(idx_t n)
{
    static seq_t *cache          = NULL;
    static size_t cache_len      = 0;
    static size_t cache_cap      = 0;

    /* Initialise cache with the first four terms, if needed. */
    if (cache == NULL) {
        cache_cap = 4;
        cache_len = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)26;
        cache[3] = (seq_t)836;
    }

    /* Ensure cache is large enough. */
    if ((size_t)n + 1 > cache_len) {
        size_t needed = (size_t)n + 1;
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        while (new_cap < needed) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;            /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;

        /* Compute missing terms. */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = compute_next(i, cache);
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
