/*
 * A158513  a(n) = Hermite(n,5).
 * https://oeis.org/A158513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 22, i128 n <= 43,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = Hermite(n,5)
   Recurrence: a(0) = 1, a(1) = 10,
                a(n) = 10*a(n-1) - 2*(n-1)*a(n-2)   for n >= 2
   (Orthogonal polynomial H_n(5) with H_0=1, H_1=2*5=10) */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/alloc_seq_array.h"   /* decl. of alloc_seq_array */
#include "../utils/seq_free_array.h"    /* decl. of seq_free_array */

seq_t a(idx_t n)
{
    /* static memoization state */
    static seq_t *cache     = NULL;      /* array with computed values   */
    static size_t  cache_cap = 0;        /* currently allocated size     */
    static idx_t   cached_max  = (idx_t)-1; /* highest index computed so far */

    /* initialise cache for a(0)=1, a(1)=10 */
    if (cache == NULL) {
        cache_cap = 2;
        cache = alloc_seq_array(cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cached_max = 1;
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;               /* double until large enough */
        }
        seq_t *new_cache = alloc_seq_array(new_cap);
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        seq_free_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Return if already computed */
    if (n <= cached_max) {
        return cache[n];
    }

    /* Compute values up to n using recurrence */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)10 * cache[i - 1];
        seq_t term2 = (seq_t)2 * ((seq_t)(i - 1)) * cache[i - 2];
        cache[i] = term1 - term2;
    }
    cached_max = n;

    return cache[n];
}
