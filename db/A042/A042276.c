/*
 * A042276  Numerators of continued fraction convergents to sqrt(664).
 * https://oeis.org/A042276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(664).
 *
 * Recurrence:  p(-2)=0 , p(-1)=1 ,
 *              p(k)  = a_k * p(k-1) + p(k-2)   for k ≥ 0
 * where a_0 = 25 and for k ≥ 1: a_k = period[(k-1) mod 22] with
 * period = {1,3,3,5,2,2,1,1,2,1,5,1,2,1,1,2,2,5,3,3,1,50}.
 * This function returns p(n) for any n ≥ 0.  It memoises results
 * internally for speed.  The type seq_t is supplied by the build
 * system and may be an arbitrary‑precision integer; casts are used
 * only where the result is guaranteed to be small. */

#include <stdlib.h>
#include <stddef.h>

static const seq_t period[22] = {
    (seq_t)1, (seq_t)3, (seq_t)3, (seq_t)5, (seq_t)2, (seq_t)2,
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)5, (seq_t)1,
    (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)5,
    (seq_t)3, (seq_t)3, (seq_t)1, (seq_t)50
};
#define PERIOD (sizeof(period)/sizeof(period[0]))

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoised numerators   */
    static idx_t cache_size = 0;     /* number of cached entries */

    /* Return cached value if already computed. */
    if (n < cache_size)
        return cache[n];

    /* Grow cache to hold entries up to n.  realloc returns void*. */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();               /* memory allocation failure */
    cache = tmp;

    /* Set initial recurrence state based on what is already cached. */
    seq_t p_prevprev;   /* p(k-1) in the loop */
    seq_t p_prev;       /* p(k-2) in the loop */
    idx_t k_start;

    if (cache_size == 0) {          /* no cached values yet */
        p_prevprev = (seq_t)0;      /* p(-2) */
        p_prev     = (seq_t)1;      /* p(-1) */
        k_start    = 0;             /* first index to compute */
    }
    else if (cache_size == 1) {     /* only p0 cached */
        p_prevprev = (seq_t)1;      /* p(-1) for the next step */
        p_prev     = cache[0];     /* p0 */
        k_start    = 1;
    }
    else {                          /* at least p0 and p1 cached */
        p_prevprev = cache[cache_size - 2];   /* p(k-1) */
        p_prev     = cache[cache_size - 1];   /* p(k-2) */
        k_start    = cache_size;
    }

    /* Compute missing values up to index n. */
    for (idx_t k = k_start; k <= n; ++k) {
        seq_t a_k;
        if (k == 0)
            a_k = (seq_t)25;
        else
            a_k = period[(k - 1) % PERIOD];

        /* p(k) = a_k * p(k-1) + p(k-2) */
        seq_t p_k = a_k * p_prev + p_prevprev;
        cache[k] = p_k;

        /* Advance recurrence state. */
        p_prevprev = p_prev;
        p_prev     = p_k;
    }

    cache_size = new_size;          /* update cache size */
    return cache[n];
}
