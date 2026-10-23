/*
 * A121883  a(n) = (2*a(n-1)*a(n-3) + a(n-2)^2)/a(n-4), with a(1)=..=a(4)=1.
 * https://oeis.org/A121883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*a(n-1)*a(n-3) + a(n-2)^2) / a(n-4), a(1) = a(2) = a(3)
 *            = a(4) = 1
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 13, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute one term, assuming cache[0..idx-2] is already filled */
#ifndef __cplusplus
/* C implementation – rely on 128‑bit intermediate for safety  */
static inline seq_t compute_term(idx_t idx, seq_t *cache) {
    /* idx is 1‑based; we compute for idx >= 5 */
    __int128 a1 = (__int128)cache[idx-2]; /* a(n-1) */
    __int128 a2 = (__int128)cache[idx-3]; /* a(n-2) */
    __int128 a3 = (__int128)cache[idx-4]; /* a(n-3) */
    __int128 a4 = (__int128)cache[idx-5]; /* a(n-4) */

    __int128 term1 = 2 * a1 * a3;   /* 2*a(n-1)*a(n-3) */
    __int128 term2 = a2 * a2;       /* a(n-2)^2 */
    __int128 numer  = term1 + term2;
    __int128 denom  = a4;

    return (seq_t)(numer / denom);
}
#endif

#ifdef __cplusplus
/* C++ implementation – template‑based compile‑time selection */
static inline seq_t compute_term(idx_t idx, seq_t *cache) {
    /* idx is 1‑based; we compute for idx >= 5 */
    if constexpr (sizeof(seq_t) <= sizeof(unsigned long long)) {
        /* 64‑bit cache – use 128‑bit intermediates */
        __int128 a1 = (__int128)cache[idx-2];
        __int128 a2 = (__int128)cache[idx-3];
        __int128 a3 = (__int128)cache[idx-4];
        __int128 a4 = (__int128)cache[idx-5];

        __int128 term1 = 2 * a1 * a3;
        __int128 term2 = a2 * a2;
        __int128 numer  = term1 + term2;
        __int128 denom  = a4;

        return (seq_t)(numer / denom);
    } else {
        /* arbitrary‑precision cache – use native operators */
        seq_t a1 = cache[idx-2];
        seq_t a2 = cache[idx-3];
        seq_t a3 = cache[idx-4];
        seq_t a4 = cache[idx-5];

        seq_t term1 = a1 * seq_t(2) * a3;
        seq_t term2 = a2 * a2;
        seq_t numer  = term1 + term2;
        return numer / a4;
    }
}
#endif

/* Main sequence function – memoises results in a static cache */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n <= 4) return (seq_t)1;

    /* static cache state */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t last  = 0;  /* number of terms currently stored */

    /* Initialise cache if first call */
    if (cache == NULL) {
        idx_t size = (n > 4 ? n : 4);
        cache = (seq_t*)calloc((size_t)size, sizeof(seq_t));
        if (!cache) exit(1);
        for (idx_t i = 0; i < 4; ++i) cache[i] = (seq_t)1;
        cap  = size;
        last = 4;
    }
    /* Grow cache if requested size exceeds current capacity */
    else if (n > cap) {
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        /* Initialise any new entries to 1 if we just happened to grow from <4 */
        if (last < 4)
            for (idx_t i = last; i < 4; ++i) cache[i] = (seq_t)1;
        cap  = n;
        if (last < 4) last = 4;
    }

    /* Compute missing terms */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i-1] = compute_term(i, cache);
    }
    last = n;
    return cache[n-1];
}
