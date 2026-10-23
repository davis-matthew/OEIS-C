/*
 * A164390  Number of binary strings of length n with no substrings equal to
 *          0000 or 0110.
 * https://oeis.org/A164390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4) + a(n-6) (n ≥ 6) Initial values:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=14, a(5)=25
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 75, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* The function returns the number of binary strings of length n that
   avoid the substrings 0000 and 0110.  It uses memoised dynamic
   programming with a static cache that grows on demand.  The cache
   holds all previously computed values up to the largest n requested
   so far.  The recurrence a(n)=a(n-1)+a(n-2)+a(n-4)+a(n-6) handles
   n ≥ 6; the first six terms are inserted explicitly.  All arithmetic
   is performed using seq_t operators; for large builds seq_t is a
   big-integer class, while for 64‑bit builds it is an unsigned
   integer type. */

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of elements allocated */

    /* need to compute up to index n (inclusive) */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_len) {
        /* remember the previous size */
        size_t old_len = cache_len;

        /* grow the cache */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)            /* allocation failed – fall back to 0 */
            return (seq_t)0;
        cache     = tmp;
        cache_len = (idx_t)needed;

        /* initialise first six terms if they are not yet set */
        if (cache_len >= 1) cache[0] = (seq_t)1;
        if (cache_len >= 2) cache[1] = (seq_t)2;
        if (cache_len >= 3) cache[2] = (seq_t)4;
        if (cache_len >= 4) cache[3] = (seq_t)8;
        if (cache_len >= 5) cache[4] = (seq_t)14;
        if (cache_len >= 6) cache[5] = (seq_t)25;

        /* compute new values beyond what we already have */
        idx_t start = old_len > 6 ? old_len : 6;
        for (idx_t i = start; i <= n; ++i) {
            /* recurrence: a(i) = a(i-1) + a(i-2) + a(i-4) + a(i-6) */
            cache[i] = cache[i-1] + cache[i-2] + cache[i-4] + cache[i-6];
        }
    }

    return cache[n];
}
