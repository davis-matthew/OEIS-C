/*
 * A112044  Let b(0)=1/2, b(n) = (b(n-1)+Prime[n])/2; sequence gives
 *          2^(n+1)*b(n).
 * https://oeis.org/A112044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=a(n-1)+2^n * Prime[n]
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"

/* Sequence computation: a(n) = 2^(n+1) * b(n) where
   b(0)=1/2, b(n)=(b(n-1)+Prime[n])/2.  */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* cache[0] = a(0) */
    static idx_t  cap    = 0;       /* allocated capacity */
    static idx_t  sz     = 0;       /* number of stored values */

    /* Initialise cache with a(0)=1 on first call */
    if (sz == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 1;   /* a(0) */
        sz = 1;
    }

    /* Grow cache if needed and compute up to n */
    while (sz <= n) {
        /* Compute a(sz) based on a(sz-1) */
        seq_t prev = cache[sz - 1];

        /* Prime[sz]  (Prime[1] = 2, Prime[2] = 3, ...) */
        seq_t p = get_prime(sz);

        /* 2^sz */
        seq_t two_pow = pow2(sz);

        /* a(sz) = prev + two_pow * p */
        seq_t add = two_pow * p;
        seq_t cur = prev + add;

        /* Ensure capacity */
        if (sz == cap) {
            cap <<= 1;  /* double */
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }

        cache[sz++] = cur;
    }

    return cache[n];
}
