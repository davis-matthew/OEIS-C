/*
 * A065922  Bessel polynomial {y_n}'(4).
 * https://oeis.org/A065922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n‑1)^2*a(n) = (2*n-1)^3*a(n-1) + n^2*a(n-2) a(0) = 0, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* dynamic cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_next = 0;       /* next index that needs to be computed */
static size_t cache_cap  = 0;       /* current allocated size */

/* ensure cache can contain indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < required)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* out of memory */
    cache     = tmp;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* a(0) = 0 */

    /* initialise cache on first call */
    if (!cache) {
        cache_cap   = 2;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();                /* OOM */
        cache[0] = 0;                /* a(0) */
        cache[1] = 1;                /* a(1) */
        cache_next = 2;               /* first index to compute */
    }

    ensure_cache(n);

    /* compute missing terms up to n */
    for (idx_t i = cache_next; i <= n; ++i) {
        /* compute left side denominator: (i-1)^2 */
        /* use 128‑bit intermediate for small seq_t to avoid overflow */
#if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ >= 16
        if (sizeof(seq_t) <= 8) {
            /* all literals below fit in 64 bits */
            uint64_t mul1 = 2*i - 1;
            mul1 *= mul1 * mul1;            /* (2i-1)^3 < 1e13  */
            uint64_t mul2 = i * i;          /* i^2 < 1e8       */

            __uint128_t left  = (__uint128_t)(i-1) * (i-1);
            __uint128_t term1 = (__uint128_t)mul1 * (__uint128_t)cache[i-1];
            __uint128_t term2 = (__uint128_t)mul2 * (__uint128_t)cache[i-2];
            __uint128_t rhs   = term1 + term2;
            cache[i] = (seq_t)(rhs / left);
        } else
#endif
        {
            /* arbitrary‑precision or 128‑bit seq_t */
            uint64_t mul1 = 2*i - 1;
            mul1 *= mul1 * mul1;            /* (2i-1)^3 */
            uint64_t mul2 = i * i;          /* i^2 */

            seq_t left  = (seq_t)(i-1) * (i-1);
            seq_t term1 = (seq_t)mul1 * cache[i-1];
            seq_t term2 = (seq_t)mul2 * cache[i-2];
            seq_t rhs   = term1 + term2;
            cache[i]    = rhs / left;       /* exact division */
        }
    }

    cache_next = n + 1;
    return cache[n];
}
