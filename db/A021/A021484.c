/*
 * A021484  Expansion of 1/((1-x)*(1-3*x)*(1-5*x)*(1-12*x)).
 * https://oeis.org/A021484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021484: 1/((1-x)*(1-3*x)*(1-5*x)*(1-12*x))
   Recurrence: a(0)=1, a(1)=21, a(2)=310, a(3)=4050
                a(n)=21*a(n-1)-131*a(n-2)+291*a(n-3)-180*a(n-4)  (n>3) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)21;
    if (n == 2) return (seq_t)310;
    if (n == 3) return (seq_t)4050;

    /* Static cache to allow arbitrary order of calls */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* allocated size */
    static size_t   sz    = 0;   /* number of terms stored */

    /* Initialise the cache with the first four values */
    if (cap == 0) {
        cap = 8;                         /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)310;
        cache[3] = (seq_t)4050;
        sz = 4;
    }

    /* If we already have the value, return it immediately */
    if (n < sz) return cache[n];

    /* Ensure the cache is large enough for the requested index */
    if (n >= cap) {
        size_t newcap = cap;
        while (n >= newcap) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute the sequence up to the requested index */
    for (idx_t i = sz; i <= n; ++i) {
        seq_t term =
            cache[i-1] * (seq_t)21
          - cache[i-2] * (seq_t)131
          + cache[i-3] * (seq_t)291
          - cache[i-4] * (seq_t)180;
        cache[i] = term;
    }
    sz = n + 1;
    return cache[n];
}
