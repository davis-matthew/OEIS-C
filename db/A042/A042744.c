/*
 * A042744  Numerators of continued fraction convergents to sqrt(903).
 * https://oeis.org/A042744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A042744 – Numerators of continued fraction convergents to sqrt(903)
   Formula: a(0)=30, a(1)=601, a(2)=36090, a(3)=722401,
   a(n)=1202*a(n-2)-a(n-4)  for n ≥ 4
*/
#include <stdlib.h>

static seq_t *cache = NULL;         /* cache of computed terms */
static size_t cache_len = 0;        /* number of entries currently stored */

seq_t a(idx_t n)
{
    /* base values – return directly if requested */
    if (n == 0) return (seq_t)30;
    if (n == 1) return (seq_t)601;
    if (n == 2) return (seq_t)36090;
    if (n == 3) return (seq_t)722401;

    /* initialise the cache if it does not yet exist */
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t*)calloc(cache_len, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)30;
        cache[1] = (seq_t)601;
        cache[2] = (seq_t)36090;
        cache[3] = (seq_t)722401;
    }

    /* remember the size of the cache before any growth */
    size_t old_len = cache_len;

    /* grow the cache to hold up to index n if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = new_len;
    }

    /* compute any missing terms using the recurrence */
    for (idx_t i = (idx_t)old_len; i <= n; ++i) {
        cache[(size_t)i] =
            (seq_t)1202 * cache[(size_t)(i-2)] - cache[(size_t)(i-4)];
    }

    return cache[(size_t)n];
}
