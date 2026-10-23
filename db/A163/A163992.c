/*
 * A163992  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=24, a(2)=552, a(3)=12696, a(4)=292008, a(5)=6716184
 *            For n ≥ 6 a(n) = 22 * (a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)) - 253
 *            * a(n-6) + (n==6 ? 1 : 0) // the numerator contributes +1 only
 *            for n=6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const seq_t init_vals[6] = {
    (seq_t)1ULL,          /* a(0) */
    (seq_t)24ULL,         /* a(1) */
    (seq_t)552ULL,        /* a(2) */
    (seq_t)12696ULL,      /* a(3) */
    (seq_t)292008ULL,     /* a(4) */
    (seq_t)6716184ULL     /* a(5) */
};

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* number of stored terms */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* For n less than 6, return the constant table directly */
    if (n < 6)
        return init_vals[n];

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 64;                       /* initial allocation */
        cache     = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL)
            abort();
    }

    /* Seed the first six values if not already present */
    if (cache_len < 6) {
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init_vals[i];
        cache_len = 6;
    }

    /* Grow the cache if n is beyond current capacity */
    while ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Sum the five preceding terms */
        seq_t sum = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5];
        seq_t val = (seq_t)22 * sum;
        val -= (seq_t)253 * cache[i-6];
        if (i == 6)      /* the special +1 term from the numerator */
            val += (seq_t)1;
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
