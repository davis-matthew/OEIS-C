/*
 * A098577  a(n) = Sum_{k=0..floor(n/5)} C(n-3*k,2*k) * 2^k.
 * https://oeis.org/A098577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-5) with initial values:
 *            a(0)=a(1)=a(2)=a(3)=a(4)=1
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 46, u64 n <= 89, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed terms      */
static idx_t  cache_len = 0;     /* allocated length             */
static idx_t  computed_len = 0;  /* highest index that has been computed (inclusive) */

/* Extend the cache so that all terms up to and including n are available */
static void ensure_cache(idx_t n)
{
    if (n < computed_len)            /* everything already computed */
        return;

    /* Allocate or grow the storage as required */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (cache == NULL) return;   /* allocation failure – caller will fail */
        cache_len = n + 1;
    } else if (cache_len <= n) {
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (cache == NULL) return;
    }

    /* Compute any missing terms one by one, using the recurrence */
    for (idx_t i = computed_len; i <= n; ++i) {
        if (i < 5) {
            cache[i] = (seq_t)1;          /* base values */
        } else {
            /* a(i) = 2*a(i-1) - a(i-2) + 2*a(i-5)    */
            seq_t term = cache[i - 1] << 1;    /* 2*a(i-1) */
            term -= cache[i - 2];             /* subtract a(i-2) */
            term += (cache[i - 5] << 1);      /* add 2*a(i-5) */
            cache[i] = term;
        }
    }
    computed_len = n;
}

/* Public entry point */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
