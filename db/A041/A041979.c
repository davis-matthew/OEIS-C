/*
 * A041979  Denominators of continued fraction convergents to sqrt(512).
 * https://oeis.org/A041979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 18, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A041979 – Denominators of convergents to √512.
 *
 * Recurrence:
 *     a(0) = 1, a(1) = 1
 *     a(n) = t[n‑1] · a(n‑1) + a(n‑2)   (n ≥ 2)
 *
 * Where the coefficient sequence t repeats every 12 terms:
 *     t = {1,1,1,2,6,11,6,2,1,1,1,44}
 */

#include <stdlib.h>

/* periodic coefficient pattern */
static const uint64_t pattern[12] = {1,1,1,2,6,11,6,2,1,1,1,44};

seq_t a(idx_t n)
{
    /* if the first term is requested, return immediately */
    if (n == 0) return (seq_t)1;

    /* cached values: cache[0]=1, cache[1]=1.  Allocate on first use. */
    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;   /* allocated capacity   */
    static idx_t  last    = 0;   /* highest index computed */

    if (cache == NULL) {
        cap   = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last    = 1;
    }

    /* if the requested value is already cached, return it */
    if (n <= last) return cache[n];

    /* grow the cache if necessary */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* compute missing terms in order using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t coeff = (seq_t)(pattern[(i - 1) % 12]);
        cache[i] = coeff * cache[i - 1] + cache[i - 2];
    }

    last = n;
    return cache[n];
}
