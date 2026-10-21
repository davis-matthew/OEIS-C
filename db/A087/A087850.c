/*
 * A087850  a(n) = a(a(a(a(a(n-1))))) + abs(2*a(n-1)-n).
 * https://oeis.org/A087850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, fini
 * Formula:   a(n) = a(a(a(a(a(n-1))))) + abs(2*a(n-1)-n)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial terms: a(1)=1, a(2)=1, a(3)=2, a(4)=1, ...       */

#include <stdlib.h>
#include <stddef.h>

/* static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* allocated array length (0 means none) */

/* ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* we use indices 0..n, so size must be > n */
    if (cache_len <= (size_t)n) {
        size_t new_len = cache_len ? cache_len * 2 : 4;
        while (new_len <= (size_t)n)
            new_len <<= 1;           /* grow until it exceeds n */

        seq_t *tmp;
#if defined(__cplusplus)
        tmp = reinterpret_cast<seq_t*>(realloc(cache, new_len * sizeof(seq_t)));
#else
        tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
#endif
        if (!tmp && new_len > 0)
            abort();                  /* allocation failure */
        /* initialise the newly allocated portion to zero */
        for (size_t i = cache_len; i < new_len; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_len = new_len;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n < 1 */

    ensure_cache(n);

    if (cache[n] != 0)
        return cache[n];               /* memoised value */

    if (n == 1) {                      /* base case */
        cache[1] = 1;
        return 1;
    }

    /* recursive computation with memoisation */
    seq_t t1 = a(n - 1);                           /* a(n-1) */
    seq_t t2 = a((idx_t)t1);                       /* a(a(n-1)) */
    seq_t t3 = a((idx_t)t2);                       /* a(a(a(n-1))) */
    seq_t t4 = a((idx_t)t3);                       /* a(a(a(a(n-1)))) */
    seq_t t5 = a((idx_t)t4);                       /* a(a(a(a(a(n-1))))) */

    seq_t abs_term = (seq_t)2 * t1 > n ?
                     (seq_t)2 * t1 - n :
                     n - (seq_t)2 * t1;

    seq_t result = t5 + abs_term;
    cache[n] = result;
    return result;
}
