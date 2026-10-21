/*
 * A203310  a(n) = A203309(n+1)/A203309(n).
 * https://oeis.org/A203310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! * (2*n+2)!)/(2^n * (n+2)!)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* recurrence: a(0) = 1 ; a(n) = a(n-1) * n*(n+1)*(2n+1) / (n+2) */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values stored */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache for the first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;          /* out of memory */
        cache[0] = (seq_t)1;                 /* a(0) */
        cache_len = 1;
    }

    /* grow the cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n + 1 > cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = cache[i - 1];          /* a(i-1) */

            /* factors of the recurrence */
            seq_t denom = (seq_t)(i + 2);
            seq_t f0 = (seq_t)i;
            seq_t f1 = (seq_t)(i + 1);
            seq_t f2 = (seq_t)(2 * i + 1);

            /* cancel common factors with denom first */
            seq_t g = gcd(f0, denom); denom /= g; f0 /= g;
            g = gcd(f1, denom); denom /= g; f1 /= g;
            g = gcd(f2, denom); denom /= g; f2 /= g;

            /* product of the remaining numerators */
            seq_t prod = f0 * f1 * f2;

            /* compute a(i) = val * prod / denom */
            if (sizeof(seq_t) <= 8) {                 /* 64‑bit or smaller */
                /* use 128‑bit intermediate to avoid overflow */
                unsigned long long prod_ll =
                    (unsigned long long)f0 *
                    (unsigned long long)f1 *
                    (unsigned long long)f2;
                unsigned __int128 tmp = (unsigned __int128)val * prod_ll;
                tmp /= (unsigned __int128)denom;
                val = (seq_t)tmp;
            } else {
                /* for 128‑bit or big integer types */
                val = (val * prod) / denom;
            }

            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
