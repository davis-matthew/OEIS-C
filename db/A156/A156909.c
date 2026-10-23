/*
 * A156909  G.f.: A(x) = 1 + x*exp( Sum_{k>=1} [A(-(-1)^k*x) - 1]^k/k ).
 * https://oeis.org/A156909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A156909 – D-finite recurrence *
 *            288*(n-6)*(n-5)*(n-4)*(n-3)*a(n-5) +
 *            24*(n-4)*(n-3)*(52*n^2-378*n+761)*a(n-3) * +
 *            2*(n-1)*(181*n^3-271*n^2-950*n+1752)*a(n-1) -
 *            (n-1)*(n+1)*(87*n^2+38*n+48)*a(n+1) * +
 *            4*(n+1)*(n+2)*(n+3)*(n-1)*a(n+3) = 0 * with initial terms
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=3,...
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 13, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external integer types supplied by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;

    /* initialise cache with first 8 terms */
    if (cache == NULL) {
        cache = (seq_t*)malloc(8 * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)3;
        cache[5] = (seq_t)10;
        cache[6] = (seq_t)18;
        cache[7] = (seq_t)70;
        cache_max = 7;
    }

    if (n <= cache_max) return cache[n];

    /* expand cache to needed size */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) abort();
    cache = new_cache;

    /* compute new terms using the recurrence */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        if (k <= 7) continue;   /* already initialised */

        /* helper variables */
        seq_t t   = (seq_t)(k - 3);
        seq_t t2  = t * t;
        seq_t t3  = t2 * t;

        /* term for a(k-8) : 288*(k-9)*(k-8)*(k-7)*(k-6)*a(k-8) */
        seq_t term1 = (seq_t)288;
        term1 *= (k - 9);
        term1 *= (k - 8);
        term1 *= (k - 7);
        term1 *= (k - 6);
        term1 *= cache[k - 8];

        /* term for a(k-6) : 24*(k-7)*(k-6)*(52*t2-378*t+761)*a(k-6) */
        seq_t poly1 = (seq_t)52 * t2;
        poly1 -= (seq_t)378 * t;
        poly1 += (seq_t)761;
        seq_t term24 = (seq_t)24;
        term24 *= (k - 7);
        term24 *= (k - 6);
        term24 *= poly1;
        term24 *= cache[k - 6];

        /* term for a(k-4) : 2*(k-4)*(181*t3-271*t2-950*t+1752)*a(k-4) */
        seq_t poly2 = (seq_t)181 * t3;
        poly2 -= (seq_t)271 * t2;
        poly2 -= (seq_t)950 * t;
        poly2 += (seq_t)1752;
        seq_t term2 = (seq_t)2;
        term2 *= (k - 4);
        term2 *= poly2;
        term2 *= cache[k - 4];

        /* term for a(k-2) : (k-4)*(k-2)*(87*t2+38*t+48)*a(k-2) */
        seq_t poly3 = (seq_t)87 * t2;
        poly3 += (seq_t)38 * t;
        poly3 += (seq_t)48;
        seq_t term3 = (seq_t)(k - 4);
        term3 *= (k - 2);
        term3 *= poly3;
        term3 *= cache[k - 2];

        /* denominator: 4*(k-2)*(k-1)*k*(k-4) */
        seq_t denom = (seq_t)4;
        denom *= (k - 2);
        denom *= (k - 1);
        denom *= k;
        denom *= (k - 4);

        /* numerator: term3 - term1 - term24 - term2 */
        seq_t numerator = term3 - term1 - term24 - term2;

        cache[k] = numerator / denom;  /* exact division */
    }

    cache_max = n;
    return cache[n];
}
