/*
 * A097819  E.g.f. exp(3x)/(1-4x).
 * https://oeis.org/A097819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097819
   Formula:  a(0) = 1
             a(1) = 7
             a(n) = (4*n + 3)*a(n-1) – 12*(n-1)*a(n-2),  n ≥ 2
   (Derived from the recurrence a(n)+(-4*n-3)a(n-1)+12*(n-1)a(n-2)=0)
*/

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values – allows out‑of‑order calls */
    static seq_t *cache   = NULL;   /* pointer to array of seq_t */
    static idx_t  cache_size = 0;   /* number of elements currently computed */
    static size_t cache_cap  = 0;   /* allocated capacity of cache */

    /* initialise cache for the first call */
    if (!cache) {
        cache_cap   = 16;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]    = (seq_t)1;      /* a(0) */
        cache[1]    = (seq_t)7;      /* a(1) */
        cache_size  = 2;
    }

    /* grow cache until it contains a(n) */
    while (cache_size <= n) {
        /* make sure we have enough capacity */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute the next term using the second‑order recurrence */
        idx_t k = cache_size; /* next index to compute */
        seq_t term = ((seq_t)(4 * k + 3)) * cache[k - 1];
        term -= ((seq_t)(12 * (k - 1))) * cache[k - 2];
        cache[k] = term;

        ++cache_size;
    }

    return cache[n];
}
