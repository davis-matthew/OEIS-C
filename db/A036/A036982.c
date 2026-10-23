/*
 * A036982  a(n) = [a*a(n-1)+b]/p^r, where a=2.001, b=3.2, p=2 and p^r is the
 *          highest power of p dividing [a*a(n-1)+b], starting with a(0) = 1.
 * https://oeis.org/A036982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 517, u32 n <= 598, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A036982: a(n) = floor((2.001*a(n‑1)+3.2)/(2^r)),
   where r is the largest exponent such that 2^r divides
   floor(2.001*a(n‑1)+3.2) and a(0)=1.  */
/* References: 2.001 = 2001/1000, 3.2 = 3200/1000.  */

#include <stdbool.h>   /* for bool if needed */
#include <stddef.h>    /* for size_t */
#include <stdlib.h>    /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t   sz   = 0;
    static idx_t   cap  = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* out of memory */
        cache[0] = 1;                  /* a(0) = 1 */
        sz = 1;
    }

    if (n < sz)                      /* already computed */
        return cache[n];

    /* extend cache up to requested index */
    for (idx_t i = sz; i <= n; ++i)
    {
#if defined(__SIZEOF_INT128__)
        /* use 128‑bit arithmetic for 64‑bit seq_t to avoid overflow */
        unsigned __int128 num = (unsigned __int128)cache[i-1] * 2001U + 3200U;
        unsigned __int128 t128 = num / 1000U;

        /* valuation of 2 in t128 */
        idx_t r = 0;
        unsigned __int128 y = t128;
        while ((y & 1U) == 0U) {
            y >>= 1U;
            ++r;
        }

        seq_t next = (seq_t)(t128 >> r);    /* cast back to seq_t */
#else
        /* no 128‑bit support – rely on seq_t to hold the product   */
        seq_t num = cache[i-1] * 2001U + 3200U;
        seq_t t   = num / 1000U;

        /* valuation of 2 in t */
        idx_t r = 0;
        seq_t y = t;
        while ((y & 1) == 0) {
            y >>= 1;
            ++r;
        }

        seq_t next = t >> r;
#endif

        /* grow cache if necessary */
        if (i >= cap) {
            idx_t newcap = cap ? cap * 2 : 32;
            cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (cache == NULL) return 0;   /* out of memory */
            cap = newcap;
        }
        cache[i] = next;
    }

    sz = n + 1;
    return cache[n];
}
