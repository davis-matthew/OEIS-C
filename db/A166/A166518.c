/*
 * A166518  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of reduced words of length n in the Coxeter group on 7 generators
   with relations S_i^2 = (S_i S_j)^12 = I.
   Recurrence (from OEIS A166518):
      a(n) = 5*∑_{j=1}^{11} a(n-j) - 15*a(n-12)
   Initial values:
      a(0)=1
      a(1)=7
      a(2)=42
      a(3)=252
      a(4)=1512
      a(5)=9072
      a(6)=54432
      a(7)=326592
      a(8)=1959552
      a(9)=11757312
      a(10)=70543872
      a(11)=423263232
      a(12)=2539579371
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache that keeps all computed terms.
       cache_size   : number of valid terms (last index + 1).
       cache_cap    : allocated capacity of the array.                 */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* Initial values for n = 0 .. 12.  Copy only once.                */
    static const seq_t base_values[13] = {
        (seq_t)1,          /* a(0)  */
        (seq_t)7,          /* a(1)  */
        (seq_t)42,         /* a(2)  */
        (seq_t)252,        /* a(3)  */
        (seq_t)1512,       /* a(4)  */
        (seq_t)9072,       /* a(5)  */
        (seq_t)54432,      /* a(6)  */
        (seq_t)326592,     /* a(7)  */
        (seq_t)1959552,    /* a(8)  */
        (seq_t)11757312,   /* a(9)  */
        (seq_t)70543872,   /* a(10) */
        (seq_t)423263232,  /* a(11) */
        (seq_t)2539579371 /* a(12) */
    };

    /* Initialise the cache on the first call.                         */
    if (cache == NULL) {
        /* Allocate a small initial capacity.  Will grow as needed.     */
        cache_cap  = 64;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed      */
        memcpy(cache, base_values, sizeof(base_values));
        cache_size = 13;                      /* indices 0..12 valid   */
    }

    /* If the requested index is already known, return it.             */
    if ((size_t)n < cache_size) return cache[(size_t)n];

    /* Grow the cache to be able to store up to index n.                */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;   /* allocation failed  */
        cache = new_cache;
        cache_cap = new_cap;
        /* The new elements are uninitialised; they will be overwritten. */
    }

    /* Compute missing terms using the recurrence.                     */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        for (int j = 1; j <= 11; ++j) sum += cache[i - j];
        cache[i] = 5 * sum - 15 * cache[i - 12];
    }

    /* Update the number of stored terms.                               */
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
