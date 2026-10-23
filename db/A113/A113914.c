/*
 * A113914  (1,2,3) Jasinski-like positive power sequence.
 * https://oeis.org/A113914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 14, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(1)=1; for n≥1  a(n+1)=least prime p such that p=2*a(n)+3^k for some k>0 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isprime_big.h"   /* isprime_big(seq_t) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset, return 0 for safety */

    /* cache of already computed terms; a(1) is stored at cache[0] */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated slots */
    static idx_t  cache_max  = 0;   /* number of terms filled in cache */

    /* allocate or grow cache so that it can hold n terms */
    if (cache == NULL) {
        cache_cap = (size_t)(n > 16 ? n : 16);
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure, return 0 */
        cache[0] = 1;                  /* a(1) */
        cache_max = 1;
    } else if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure, return 0 */
        cache    = tmp;
        cache_cap = new_cap;
    }

    /* extend cache up to requested index n */
    for (idx_t i = cache_max; i < n; ++i) {
        seq_t current = cache[i-1];          /* a(i) stored at cache[i-1] */
        seq_t base = current * (seq_t)2;     /* 2*a(i) */
        seq_t power = (seq_t)3;              /* 3^1 */
        seq_t next;

        /* find smallest k>0 with base+3^k prime */
        while (true) {
            next = base + power;
            if (isprime_big(next))           /* works for arbitrary precision */
                break;
            power *= (seq_t)3;
        }
        cache[i] = next;                     /* a(i+1) at cache[i] */
    }

    cache_max = n;       /* now cache holds terms 1..n */
    return cache[n-1];
}
