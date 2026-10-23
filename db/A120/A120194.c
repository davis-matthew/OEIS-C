/*
 * A120194  a(n) = ceiling( Sum_{i=1..n-1} a(i)/8 ), a(1)=1.
 * https://oeis.org/A120194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; 
   a(n) = ceil( (1/8) * sum_{i=1}^{n-1} a(i) ),  n >= 2   */
#include "../utils/seq_realloc.h"    /* seq_realloc */

seq_t a(idx_t n)
{
    /* cache: array a[1 .. cache_size]  (index 0 unused) */
    static seq_t *cache      = NULL;          /* pointer to cached values          */
    static idx_t  cache_size = 0;             /* largest n computed so far         */
    static size_t cache_cap  = 0;             /* allocated array capacity          */
    static seq_t  cumulative = 0;             /* sum_{i=1}^{cache_size} a(i)       */

    /* Initialise on first call. */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;            /* +1 : index 0 unused */
        cache     = seq_realloc(NULL, 0, cache_cap);
        if (cache == NULL) {                 /* allocation failed – abort     */
            abort();
        }
        cache[1] = (seq_t)1;
        cumulative = (seq_t)1;
        cache_size = 1;
    }

    /* Extend cache if needed. */
    if (n > cache_size) {
        size_t new_cap = (size_t)n + 1;
        cache = seq_realloc(cache, cache_cap, new_cap);
        if (cache == NULL) { abort(); }
        cache_cap = new_cap;

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* a(i) = ceil( cumulative / 8 )   */
            seq_t ai = cumulative + (seq_t)7;  /* +7 gives ceil for positive divisors */
            ai /= (seq_t)8;                   /* integer division */
            cache[i] = ai;
            cumulative += ai;
        }
        cache_size = n;
    }

    return cache[n];
}
