/*
 * A052942  Expansion of 1/((1+x)*(1-2*x+2*x^2-2*x^3)).
 * https://oeis.org/A052942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-4), a(0)=a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 52, u64 n <= 103,
 *            u128 n <= 206, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function uses a static cache to compute terms once.
   Because the function may be called in arbitrary order, we
   grow the cache dynamically. The recurrence uses only
   addition and multiplication by a small constant, so the
   growth is O(n).  For integer builds this will wrap
   modulo 2^bits, and for the arbitrary‑precision build
   the exact value will be produced.  */
static seq_t *cache = NULL;
static idx_t cache_size = 0; /* number of terms currently in cache */
static idx_t cache_cap  = 0; /* allocated capacity of the cache */

seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 3) {
            cache[i] = (seq_t)1;
        } else {
            cache[i] = cache[i-1] + ((seq_t)2 * cache[i-4]);
        }
    }
    cache_size = n + 1;
    return cache[n];
}
