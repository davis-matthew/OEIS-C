/*
 * A066976  a(0) = 1; for n>0: a(n) = sum{a(i)*n^i : 0<=i<n}.
 * https://oeis.org/A066976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Σ_{i=0}^{n-1} a(i)·n^i, a(0)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/*  Computes the nth term of OEIS sequence A066976.  */
seq_t a(idx_t n)
{
    /* Static cache for memoisation.  */
    static seq_t *cache   = NULL;  /* array of previously computed terms */
    static size_t cache_len = 0;   /* number of values stored */
    static size_t cache_cap = 0;   /* allocated capacity (bytes of array) */

    /* Initialise cache with a(0)=1 on first call.  */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();            /* allocation failure */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* If result already known, return it.  */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Grow cache if necessary.  */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        /* Double until we hold up to n (inclusive).  */
        while ((size_t)n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n.  */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        seq_t pow = (seq_t)1;           /* k^i, starting with i=0 */
        for (idx_t i = 0; i < k; ++i) {
            sum += cache[i] * pow;
            pow *= (seq_t)k;
        }
        cache[k] = sum;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
