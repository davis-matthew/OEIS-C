/*
 * A041327  Denominators of continued fraction convergents to sqrt(177).
 * https://oeis.org/A041327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 124846*a(n-8) - a(n-16) for n ≥ 16
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* local copy of the first 16 terms (offset 0) */
static const seq_t init_vals[16] = {
    (seq_t)1,          /* a(0) */
    (seq_t)3,          /* a(1) */
    (seq_t)10,         /* a(2) */
    (seq_t)23,         /* a(3) */
    (seq_t)194,        /* a(4) */
    (seq_t)411,        /* a(5) */
    (seq_t)1427,       /* a(6) */
    (seq_t)4692,       /* a(7) */
    (seq_t)123419,     /* a(8) */
    (seq_t)374949,     /* a(9) */
    (seq_t)1248266,    /* a(10) */
    (seq_t)2871481,    /* a(11) */
    (seq_t)24220114,   /* a(12) */
    (seq_t)51311709,   /* a(13) */
    (seq_t)178155241,  /* a(14) */
    (seq_t)585777432   /* a(15) */
};

seq_t a(idx_t n)
{
    /* Return a term that is already known without allocating the cache */
    if (n < 16)
        return init_vals[n];

    /* Memoisation cache (dynamic, expandable) */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* capacity in elements          */
    static idx_t last = 15;     /* highest index computed so far */

    /* Allocate cache and copy the initial values if this is the first call */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;   /* allocation failure, return zero as a sentinel */
        for (idx_t i = 0; i < 16; ++i)
            cache[i] = init_vals[i];
    }

    /* Grow the cache if the requested index is outside current capacity */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while ((size_t)(n + 1) > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any missing terms using the linear recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prod = cache[i - 8] * (seq_t)124846;
        cache[i]   = prod - cache[i - 16];
    }
    last = n;
    return cache[n];
}
