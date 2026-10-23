/*
 * A024436  Expansion of 1/((1-x)(1-6x)(1-11x)(1-12x)).
 * https://oeis.org/A024436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A024436:
   Expansion of 1/((1-x)(1-6x)(1-11x)(1-12x)).
   Recurrence: a(0)=1, a(1)=30, a(2)=601, a(3)=10122,
                a(n)=30*a(n-1)-299*a(n-2)+1062*a(n-3)-792*a(n-4) for n≥4. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    idx_t i;

    /* initialise cache (four First terms) once */
    if (cache == NULL) {
        cache_len = 4;
#if defined(__cplusplus)
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
#else
        cache = malloc(cache_len * sizeof(seq_t));
#endif
        if (cache == NULL) return (seq_t)0;         /* allocation error */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)30;
        cache[2] = (seq_t)601;
        cache[3] = (seq_t)10122;
    }

    /* if the requested index is already in the cache, return it */
    if (n < cache_len) return cache[n];

    /* otherwise extend the cache up to n using the recurrence */
#if defined(__cplusplus)
    seq_t *tmp = (seq_t*)realloc(cache, (n+1) * sizeof(seq_t));
#else
    seq_t *tmp = realloc(cache, (n+1) * sizeof(seq_t));
#endif
    if (tmp == NULL) return (seq_t)0;               /* allocation error */
    cache = tmp;

    /* fill in missing terms */
    for (i = cache_len; i <= n; ++i) {
        seq_t t0 = cache[i-1] * (seq_t)30;
        seq_t t1 = cache[i-2] * (seq_t)299;
        seq_t t2 = cache[i-3] * (seq_t)1062;
        seq_t t3 = cache[i-4] * (seq_t)792;
        seq_t sum = t0 - t1 + t2 - t3;
        cache[i] = sum;
    }
    cache_len = n + 1;

    return cache[n];
}
