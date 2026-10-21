/*
 * A212824  Number of 0..3 arrays of length n with no adjacent pair equal to
 *          its immediately preceding adjacent pair, and new values introduced
 *          in 0..3 order.
 * https://oeis.org/A212824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 6*a(n-3) - 3*a(n-4) for n>7 Initial
 *            values: a(1)=1, a(2)=2, a(3)=5, a(4)=13, a(5)=43, a(6)=152,
 *            a(7)=559
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL; /* cache[1] holds a(1), etc. */
static size_t cache_cap  = 0;    /* allocated size of cache (>= cached_max+1) */
static idx_t  cached_max = 0;    /* highest n that has been computed */

/* ensure that cache can hold indices up to `need-1` */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need) new_cap <<= 1;               /* grow exponentially */
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                                 /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                       /* undefined, but safe */

    if (cache == NULL) {
        /* first call – initialise cache with the 7 known terms */
        ensure_cache_capacity(8);                      /* at least cache[7] */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)43;
        cache[6] = (seq_t)152;
        cache[7] = (seq_t)559;
        cached_max = 7;
    }

    if (n <= cached_max) return cache[(size_t)n];

    /* grow the cache if necessary */
    ensure_cache_capacity((size_t)n + 1);

    /* compute terms from cached_max+1 up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        size_t si = (size_t)i;
        seq_t term = cache[si - 1] * (seq_t)4 + cache[si - 2];
        term -= cache[si - 3] * (seq_t)6;
        term -= cache[si - 4] * (seq_t)3;
        cache[si] = term;
    }
    cached_max = n;
    return cache[(size_t)n];
}
