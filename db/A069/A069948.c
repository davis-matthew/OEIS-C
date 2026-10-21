/*
 * A069948  a(n) = 1/exp(1) * Sum_{k>=0} (k+n)!^2 / k!^3.
 * https://oeis.org/A069948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence: (8*n-7)*a[n] = (24*n^3+3*n^2-26*n+4)*a[n-1] -
 *            (n-1)^2*(24*n^3-85*n^2+66*n+13)*a[n-2] +
 *            (n-1)^2*(8*n+1)*(n-2)^4*a[n-3] Initial values: a(0)=1, a(1)=5,
 *            a(2)=87
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0..len-1] holds computed terms */
static idx_t cache_len = 0;   /* current length of cache */

/* Compute terms up to index n, storing them in cache. */
static void compute_up_to(idx_t n)
{
    if (cache_len == 0) {
        /* Initialise cache with the first three values. */
        cache_len = 3;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)87;
    }

    if (n < cache_len)
        return;

    /* Grow cache to accommodate indices up to n. */
    cache = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!cache) abort();

    for (idx_t i = cache_len; i <= n; ++i) {
        if (sizeof(seq_t) <= 8) {
            /* 64‑bit or smaller: compute with signed __int128 to avoid overflow. */
            __int128 n128 = i;
            __int128 n2   = n128 * n128;
            __int128 n3   = n2 * n128;

            __int128 p1 = 24*n3 + 3*n2 - 26*n128 + 4;

            __int128 n1_2 = (n128-1)*(n128-1);
            __int128 t1  = n1_2 * (24*n3 - 85*n2 + 66*n128 + 13);

            __int128 t3  = n1_2 * (8*n128 + 1);
            __int128 n22 = n128 - 2;
            __int128 n22_2 = n22 * n22;
            __int128 n22_4 = n22_2 * n22_2;
            t3 = t3 * n22_4;

            __int128 denom = 8*n128 - 7;

            __int128 term1 = p1 * ( __int128)cache[i-1];
            __int128 term2 = t1 * ( __int128)cache[i-2];
            __int128 term3 = t3 * ( __int128)cache[i-3];

            __int128 numer = term1 - term2 + term3;
            cache[i] = (seq_t)(numer / denom);
        } else {
            /* arbitrary‑precision: use seq_t operations only. */
            seq_t n128 = (seq_t)i;
            seq_t n2   = n128 * n128;
            seq_t n3   = n2 * n128;

            seq_t p1 = (seq_t)24 * n3 + (seq_t)3 * n2 - (seq_t)26 * n128 + (seq_t)4;

            seq_t n1_2 = (n128 - (seq_t)1) * (n128 - (seq_t)1);
            seq_t t1 = n1_2 * ((seq_t)24 * n3 - (seq_t)85 * n2 + (seq_t)66 * n128 + (seq_t)13);

            seq_t t3 = n1_2 * ((seq_t)8 * n128 + (seq_t)1);
            seq_t n22 = n128 - (seq_t)2;
            seq_t n22_2 = n22 * n22;
            seq_t n22_4 = n22_2 * n22_2;
            t3 = t3 * n22_4;

            seq_t denom = (seq_t)8 * n128 - (seq_t)7;
            seq_t numer = p1 * cache[i-1] - t1 * cache[i-2] + t3 * cache[i-3];
            cache[i] = numer / denom;
        }
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
