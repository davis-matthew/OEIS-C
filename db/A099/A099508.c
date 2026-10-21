/*
 * A099508  A transform of the Jacobsthal numbers.
 * https://oeis.org/A099508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+2*a(n-2)-2*a(n-3)+a(n-4)-a(n-6) for n≥6 Base
 *            values: a(0)=0, a(1)=1, a(2)=1, a(3)=3, a(4)=3, a(5)=8, a(6)=9
 *            (these values are computed by the recurrence itself)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* Cache of computed terms a(0)…a(k) */
static size_t cache_cap = 0;       /* Current capacity of the cache      */
static idx_t  cache_last = (idx_t)-1; /* Largest index computed so far    */

/* Compute a(n) with O(n) time and O(1) additional memory beyond the cache. */
seq_t a(idx_t n)
{
    /* Initialize cache with the first seven terms if this is the first call. */
    if (cache_last == (idx_t)-1) {
        cache_cap = 7;                      /* store up to a(6) inclusive */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;         /* malloc failure → zero (unlikely) */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)3;
        cache[5] = (seq_t)8;
        cache[6] = (seq_t)9;
        cache_last = 6;
    }

    /* If requested term is already computed, return it immediately. */
    if (n <= cache_last)
        return cache[n];

    /* Grow the cache if necessary to hold up to a(n). */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* realloc failure → zero (unlikely) */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to a(n) using the recurrence. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* For i ≥ 6 the recurrence uses a(i-1) … a(i-6).  
           For i = 6 it matches the base value initialized above. */
        seq_t term = cache[i-1];                  /* a(i-1) */
        term += (seq_t)2 * cache[i-2];           /* +2*a(i-2) */
        term -= (seq_t)2 * cache[i-3];           /* -2*a(i-3) */
        term += cache[i-4];                      /* +a(i-4) */
        term -= cache[i-6];                      /* -a(i-6) */
        cache[i] = term;
    }
    cache_last = n;
    return cache[n];
}
