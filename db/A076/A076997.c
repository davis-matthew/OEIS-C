/*
 * A076997  a(1) = 1, a(n+1) is the largest square <= n*a(n).
 * https://oeis.org/A076997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n+1) = floor( sqrt( n * a(n) ) )^2 (largest square ≤
 *            n·a(n))
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 34,
 *            big any n
 * Helpers:   isqrt128, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/isqrt128.h"   /* unsigned long long isqrt128(__int128 x) */
#include "../utils/isqrt_seq.h"  /* seq_t isqrt_seq(seq_t v)          */

seq_t a(idx_t n)
{
    /* static cache: dynamically grow as needed */
    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;  /* allocated size of cache (indices 0..cap-1) */
    static idx_t  maxc    = 0;  /* largest index already computed */

    /* initialise cache on first call */
    if (!cache) {
#ifdef __cplusplus
        /* in C++ use new[] (default‑constructible seq_t) */
        cache = new seq_t[cap = 64]();          /* zero initialise */
#else
        /* in C use calloc – zero initialises */
        cache = (seq_t*)calloc(cap = 64, sizeof(seq_t));
#endif
        cache[1] = (seq_t)1;   /* a(1) = 1 */
        maxc = 1;
    }

    /* grow cache if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? cap : 64;
        while (n + 1 > new_cap) new_cap <<= 1;    /* double until sufficient */

#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap]();
        for (idx_t i = 0; i < cap; ++i) new_cache[i] = cache[i];
        delete[] cache;
#else
        seq_t *new_cache = realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;                 /* allocation failure – unlikely */
#endif
        cache = new_cache;
        cap   = new_cap;
    }

    /* return if already known */
    if (n <= maxc) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = maxc + 1; i <= n; ++i) {
        idx_t k = i - 1;                     /* the previous index */
        if (sizeof(seq_t) <= 8) {            /* 8‑byte or smaller integer types */
            unsigned __int128 prod = (unsigned __int128)k * (unsigned __int128)cache[k];
            unsigned long long root = isqrt128(prod);   /* floor sqrt */
            seq_t m = (seq_t)root;
            cache[i] = m * m;
        } else {                             /* arbitrary‑precision or larger types */
            seq_t prod = ((seq_t)k) * cache[k];
            seq_t root = isqrt_seq(prod);     /* floor sqrt for seq_t */
            cache[i] = root * root;
        }
    }

    maxc = n;
    return cache[n];
}
