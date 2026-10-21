/*
 * A052661  E.g.f. (2-3x)/((1-x)(1-x-x^2)).
 * https://oeis.org/A052661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * A001611(n-1) (n>0), a(0)=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n^3+6n^2+11n+6)*a(n)+(-2n-6)*a(n+2)+a(n+3)=0, */
/*   with a(0)=2, a(1)=1, a(2)=4. */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache      = NULL;   /* stored values */
static idx_t  cache_len  = 0;      /* next free index (i.e. highest stored + 1) */
static idx_t  cache_cap  = 0;      /* allocated capacity */

/* Grow the cache so that it can hold indices 0..n. */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > n) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                      /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n). */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache) {
        /* at least indices 0..2 must exist */
        idx_t need = n < 2 ? 2 : n;
        ensure_capacity(need);
        cache[0] = (seq_t)2;  /* a(0) */
        cache[1] = (seq_t)1;  /* a(1) */
        cache[2] = (seq_t)4;  /* a(2) */
        cache_len = 3;       /* next free index = 3 */
    } else {
        /* grow if needed */
        if (n >= cache_cap) ensure_capacity(n);
    }

    /* If we already have a value for n, simply return it. */
    if (n + 1 <= cache_len) return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        idx_t k = i - 3;                                   /* n in the recurrence */
        idx_t poly = k * k * k + 6 * k * k + 11 * k + 6;   /* n^3+6n^2+11n+6 */
        seq_t term1 = cache[i - 1] * (seq_t)(2 * k + 6);    /* (2k+6)*a(k+2) */
        seq_t term2 = cache[i - 3] * (seq_t)poly;           /* poly*a(k)   */
        cache[i] = term1 - term2;                          /* a(i) */
    }
    cache_len = n + 1;   /* we now have values up to n */
    return cache[n];
}
