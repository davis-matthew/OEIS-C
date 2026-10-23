/*
 * A177203  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=10, k=-1 and l=1.
 * https://oeis.org/A177203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A177203: a(0)=1, a(1)=10.
   Recurrence (k=-1,l=1):  a(n+1)= Σ_{p=0..n} (a(p)*a(n-p)-1) + 1  for n>=1  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of values stored */
    static idx_t  cache_cap = 0;   /* allocated capacity */

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache_len = 2;
    }

    /* extend cache capacity if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap < n + 1) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t m = cache_len; m <= n; ++m) {
        /* m==0 and m==1 are already initialized */
        if (m == 0) { cache[m] = (seq_t)1; continue; }
        if (m == 1) { cache[m] = (seq_t)10; continue; }

        idx_t t = m - 1;          /* the 'n' in the recurrence for a(m) */
        seq_t sum = 0;
        for (idx_t p = 0; p <= t; ++p) {
            seq_t prod = cache[p];
            prod *= cache[t - p];   /* a(p) * a(t-p) */
            prod -= (seq_t)1;       /* subtract k = -1  (i.e. minus 1) */
            sum += prod;
        }
        sum += (seq_t)1;           /* add l = 1  */
        cache[m] = sum;
    }

    /* update stored length */
    if (cache_len < n + 1) cache_len = n + 1;

    return cache[n];
}
