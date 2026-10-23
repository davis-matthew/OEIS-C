/*
 * A133147  a(n) = a(n-1) + Sum_{k=0..floor(log_2(n-1))} a(2^k), a(1) = 1.
 * https://oeis.org/A133147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Sum_{k=0..floor(log_2(n-1))} a(2^k), a(1) = 1
 * Range:     u8 n <= 11, u16 n <= 36, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache of computed values (1‑indexed) */
    static size_t cache_size = 0;        /* number of terms stored in cache    */

    if (n == 0)                     /* undefined for n < 1 – return 0 as sentinel */
        return (seq_t)0;

    /* if we already have the requested term, just return it */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* grow the cache to hold terms up to n */
    size_t new_size = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();           /* allocation failure – terminate */
    cache = tmp;

    /* initialise the first term if the cache was empty */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;     /* a(1) = 1 */
        cache_size = 1;
    }

    /* compute all missing terms up to n */
    for (size_t i = cache_size; i < new_size; ++i) {  /* i is zero‑based index */
        idx_t term = (idx_t)(i + 1);                  /* current term number */
        idx_t limit = term - 1;                       /* upper bound for powers of two */
        seq_t sum = 0;
        idx_t pow = 1;
        while (pow <= limit) {
            sum += cache[pow - 1];
            pow <<= 1;
        }
        cache[i] = cache[i - 1] + sum;                 /* a(term) = a(term-1) + sum */
    }

    cache_size = new_size;
    return cache[n - 1];
}
