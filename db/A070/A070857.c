/*
 * A070857  Expansion of (1+x*C)*C^4, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A070857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (13*n+12)*(2*n+2)!/(n!*(n+4)!) (equivalently a(n) =
 *            binom(2n+2,n)*(13*n+12)/((n+3)(n+4)) ) Recurrence: a(0)=1, for
 *            n>=1 a(n)=a(n-1)*2*(13*n+12)*(2*n+1)*(n+1) / (n*(n+4)*(13*n-1))
 *            (integer division)
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* Provides seq_t gcd(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* ensure cache has room up to max(n, cache_len) */
    if (cache_len==0) {
        cache_cap = n+1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_len = 1;
    } else if (n >= cache_cap) {
        cache_cap = n+1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* compute from the last cached index up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i-1];

        /* compute numerator and denominator of the fraction */
        seq_t num = (seq_t)0;
        seq_t den = (seq_t)0;

        /* Use 128-bit intermediate when seq_t is a builtin integer <= 64‑bit */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 tnum = 2;
            tnum *= (unsigned __int128)(13*i+12);
            tnum *= (unsigned __int128)(2*i+1);
            tnum *= (unsigned __int128)(i+1);
            num = (seq_t)tnum;

            unsigned __int128 tden = (unsigned __int128)i;
            tden *= (unsigned __int128)(i+4);
            tden *= (unsigned __int128)(13*i-1);
            den = (seq_t)tden;
        } else {
            num = (seq_t)2
                * (seq_t)(13*i+12)
                * (seq_t)(2*i+1)
                * (seq_t)(i+1);
            den = (seq_t)i
                * (seq_t)(i+4)
                * (seq_t)(13*i-1);
        }

        /* Reduce the fraction with gcds to avoid overflow */
        seq_t g1 = gcd(prev, den);
        prev /= g1;
        den  /= g1;

        seq_t g2 = gcd(num, den);
        num /= g2;
        den  /= g2;

        /* Now den should be 1, but if not, division still ok */
        seq_t next;
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 prod = (unsigned __int128)prev * (unsigned __int128)num;
            next = (seq_t)(prod / den);
        } else {
            next = prev * num / den;
        }

        cache[i] = next;
    }

    cache_len = n+1;
    return cache[n];
}
