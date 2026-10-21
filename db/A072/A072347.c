/*
 * A072347  If n = pqr...st in binary, a(n) = value of continuant
 *          [p,q,r,...,s,t].
 * https://oeis.org/A072347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 4095, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A072347:  if n = p q r ... t in binary, a(n)=continuant [p,q,r,...,t]
   Recurrence:
     a(0)=1,  a(1)=1
     a(2k)   = a(k>>1)
     a(4k+1) = a((k<<1)+1)          = a[(n+1)/2]
     a(4k+3) = a((k<<1)+1) + a[k]   = a[(n-1)/2] + a[(n-3)/4]
   The values are always positive, so zero in the cache can be used as “not cached”. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation table */
static idx_t  cache_cap = 0;     /* allocated size of cache */

static seq_t a_calc(idx_t n)
{
    /* initialise cache on first use */
    if (cache == NULL) {
        /* start with space for 2 entries: 0 and 1 */
        cache_cap = 4;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – value undefined */
        cache[0] = 1;                  /* a(0) = 1 */
        cache[1] = 1;                  /* a(1) = 1 */
    }

    /* return cached value if present */
    if (n < cache_cap && cache[n] != 0)
        return cache[n];

    seq_t val;
    if (n == 0 || n == 1) {
        val = 1;
    } else if ((n & 1) == 0) {          /* even: a(2k) = a(k>>1) */
        val = a_calc(n >> 2);
    } else if ((n & 3) == 1) {          /* n = 4k+1 : a[(n+1)/2] */
        val = a_calc(((n + 1) >> 1));
    } else {                            /* n = 4k+3 : a[(n-1)/2] + a[(n-3)/4] */
        val = a_calc(((n - 1) >> 1));
        val += a_calc(((n - 3) >> 2));
    }

    /* grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return val;     /* allocation failure – best effort */
        /* initialise new slots to 0 */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    cache[n] = val;
    return val;
}

/* public interface as required */
seq_t a(idx_t n) { return a_calc(n); }
