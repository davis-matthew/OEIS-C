/*
 * A041668  Numerators of continued fraction convergents to sqrt(353).
 * https://oeis.org/A041668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(353).
   a(0)=18, a(1)=19.
   For n≥2:  a(n) = p[n] * a(n-1) + a(n-2),
   where p[n] is the partial quotient for the n‑th step of the
   continued fraction of sqrt(353).  The sequence of p repeats
   with period 15:
   {1,3,1,2,1,1,1,1,1,1,2,1,3,1,36}.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Periodic sequence of partial quotients of sqrt(353). */
    static const seq_t period[15] = {
        (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)2, (seq_t)1,
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)2, (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)36
    };

    /* Cache for already computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with a(0) and a(1). */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        if (cache == NULL) { /* allocation failure: abort */ }
        cache[0] = (seq_t)18;
        cache[1] = (seq_t)19;
    }

    /* Return immediately if requested value is already cached. */
    if (n < cache_len)
        return cache[n];

    /* Keep track of the length before extending. */
    idx_t old_len = cache_len;

    /* Extend the cache until it can hold index n. */
    while (cache_len <= n) {
        idx_t new_len = cache_len * 2;
        if (new_len <= cache_len) new_len = cache_len + 1; /* safety fallback */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (tmp == NULL) { /* allocation failure: abort */ }
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute new terms from old_len up to n. */
    for (idx_t i = old_len; i <= n; ++i) {
        seq_t coeff = period[(i - 1) % 15];
        cache[i] = coeff * cache[i - 1] + cache[i - 2];
    }

    /* Update the effective cache length. */
    cache_len = n + 1;

    return cache[n];
}
