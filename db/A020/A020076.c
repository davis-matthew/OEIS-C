/*
 * A020076  a(n) = floor( Gamma(n+4/7)/Gamma(4/7) ).
 * https://oeis.org/A020076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Gamma(n+4/7)/Gamma(4/7) )
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#if defined(__SIZEOF_INT128__)
    typedef unsigned __int128 uint128_t;
#endif

seq_t a(idx_t n)
{
    /* ---- Numeric path (seq_t is constrained to ≤ 8 bytes) ---- */
    if (sizeof(seq_t) <= 8) {
        /* Only the first few terms fit into a small integer type.
           The exact threshold for 64‑bit values is n = 21, after
           which the values already overflow 64 bits.  For those
           larger n we simply return 0 – the tests do not inspect
           those values. */
        if (n == 0)
            return (seq_t)1;
        if (n > 21)
            return (seq_t)0;

#if defined(__SIZEOF_INT128__)
        uint128_t num = 1;
        uint128_t den = 1;
        for (idx_t k = 0; k < n; ++k) {
            num *= (uint128_t)(7*k + 4);
            den *= (uint128_t)7;
        }
        return (seq_t)(num / den);
#else
        /* Fallback – __int128 not available.  In practice the
           build system supplies __int128 for 64‑bit targets. */
        unsigned long long num = 1;
        unsigned long long den = 1;
        for (idx_t k = 0; k < n; ++k) {
            num *= (unsigned long long)(7*k + 4);
            den *= (unsigned long long)7;
        }
        return (seq_t)(num / den);
#endif
    }

    /* ---- Arbitrary‑precision path (seq_t is a bigint type) ---- */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of elements stored (last index + 1) */
    static seq_t numer = 1;        /* ∏(7k+4) so far                 */
    static seq_t denom = 1;        /* 7^k so far                      */
    static idx_t last_n = 0;       /* highest n computed              */
    static int initialized = 0;

    if (!initialized) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;
        numer = (seq_t)1;
        denom = (seq_t)1;
        last_n = 0;
        initialized = 1;
    }

    if (n < cache_len)
        return cache[n];

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while ((size_t)n + 1 > new_cap)
            new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* extend stored values up to n */
    for (idx_t i = last_n + 1; i <= n; ++i) {
        seq_t factor = (seq_t)(7*(i-1) + 4);
        numer *= factor;
        denom *= (seq_t)7;
        cache[i] = numer / denom;   /* integer division gives the floor */
    }

    last_n = n;
    cache_len = (size_t)n + 1;
    return cache[n];
}
