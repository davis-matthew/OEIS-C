/*
 * A033976  Trajectory of 1 under map n->41n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n+1)= (a(n) even)? a(n)/2 : 41*a(n)+1
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 24, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache   = NULL;   /* cached values of the sequence       */
static idx_t cache_len = 0;     /* number of elements stored          */
static idx_t cache_cap = 0;     /* current capacity of the cache      */

seq_t a(idx_t n)
{
    /* initialise cache with the first term 1 */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) abort();          /* OOM guard */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* return value if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow the cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;    /* double until big enough */
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (cache == NULL) abort();           /* OOM guard */
        cache_cap = new_cap;
    }

    seq_t mult = (seq_t)41;
    seq_t one  = (seq_t)1;

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next = (prev & one) ? prev * mult + one : prev / 2;
        cache[i] = next;
    }
    cache_len = n + 1;

    return cache[n];
}
