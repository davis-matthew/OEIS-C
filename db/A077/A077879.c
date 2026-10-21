/*
 * A077879  Expansion of (1-x)^(-1)/(1-2*x^2-2*x^3).
 * https://oeis.org/A077879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 77, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077879 – Expansion of (1-x)^(-1)/(1-2*x^2-2*x^3).
   The coefficients satisfy
        a(0)=1 , a(1)=1 , a(2)=3 , a(3)=5
        a(n)=a(n-1)+2*a(n-2)-2*a(n-4)   for n≥4
   (This follows from the GF 1/(1-2x^2-2x^3) = Σ d_k x^k with
    d_k=2*d_{k-2}+2*d_{k-3},  d_0=1, d_{k<0}=0, and a(n)=Σ_{k=0}^{n} d_k.) */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache holds values up to index n. */
    idx_t need = n + 1; /* number of elements required */

    /* Allocate or grow the cache if necessary. */
    if (cache == NULL) {
        cache_cap = need > 4 ? need : 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        /* initialise first four terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)5;
        cache_len = 4;
    } else if (need > cache_cap) {
        /* grow by factor of 2 or to the required size, whichever is larger */
        if (cache_cap < need * 2)
            cache_cap = need * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* Compute new values up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term;
        /* For i>=4 we apply the recurrence; otherwise we already have the value. */
        if (i < 4) {
            /* values 0..3 are already set in the initialization. */
            term = cache[i];
        } else {
            term = cache[i - 1]
                 + ((seq_t)2 * cache[i - 2])
                 - ((seq_t)2 * cache[i - 4]);
        }
        cache[i] = term;
    }
    cache_len = need;
    return cache[n];
}
