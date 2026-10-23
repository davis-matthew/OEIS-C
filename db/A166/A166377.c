/*
 * A166377  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=13, a(2)=156, a(3)=1872, a(4)=22464, a(5)=269568,
 *            a(6)=3234816, a(7)=38817792, a(8)=465813504, a(9)=5589762048,
 *            a(10)=67077144576, a(11)=804925734834, a(12)=9659108817072; for
 *            n≥13: a(n)=12*a(n-1)-77*a(n-11)+66*a(n-12)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Initial values a(0)..a(12) */
    static const seq_t init[13] = {
        (seq_t)1,                  /* a(0) */
        (seq_t)13,                 /* a(1) */
        (seq_t)156,                /* a(2) */
        (seq_t)1872,               /* a(3) */
        (seq_t)22464,              /* a(4) */
        (seq_t)269568,             /* a(5) */
        (seq_t)3234816,            /* a(6) */
        (seq_t)38817792,           /* a(7) */
        (seq_t)465813504,          /* a(8) */
        (seq_t)5589762048,         /* a(9) */
        (seq_t)67077144576,        /* a(10) */
        (seq_t)804925734834,       /* a(11) */
        (seq_t)9659108817072      /* a(12) */
    };

    if (n < 13) {
        return init[(size_t)n];
    }

    /* Lazy memoisation */
    static seq_t *cache = NULL;
    static idx_t  cached_last = 0;   /* highest n already computed */
    static size_t cache_cap  = 0;    /* allocated capacity */

    /* Allocate or grow cache to needed size */
    size_t needed = (size_t)n + 1;
    if (cache == NULL) {
        size_t init_cap = 13;
        if (needed > init_cap) init_cap = needed;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 13; ++i) cache[i] = init[i];
        cached_last = 12;
        cache_cap  = init_cap;
    } else if (cache_cap < needed) {
        size_t new_cap = cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    idx_t start = cached_last + 1;
    if (start < 13) start = 13;   /* values 0..12 already present via init */

    for (idx_t k = start; k <= n; ++k) {
        seq_t term =
            ( (seq_t)12 * cache[(size_t)(k-1)] )
          - ( (seq_t)77 * cache[(size_t)(k-11)] )
          + ( (seq_t)66 * cache[(size_t)(k-12)] );
        cache[(size_t)k] = term;
    }
    cached_last = n;

    return cache[(size_t)n];
}
