/*
 * A041154  Numerators of continued fraction convergents to sqrt(87).
 * https://oeis.org/A041154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 56*a(n-2) - a(n-4) * with a(0)=9, a(1)=28, a(2)=513,
 *            a(3)=1567
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of already computed terms   */
    static size_t cap   = 0;      /* allocated capacity (in seq_t slots) */
    static size_t len   = 0;      /* number of terms stored (last index+1) */

    /* initialise base values once */
    if (len == 0) {
        /* store the four starting terms */
        size_t init = 4;
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        cap   = init;
        cache[0] = (seq_t)9;
        cache[1] = (seq_t)28;
        cache[2] = (seq_t)513;
        cache[3] = (seq_t)1567;
        len = init;                        /* cache[0..3] are valid           */
    }

    /* if n is already known, return it immediately */
    if (n < len)
        return cache[n];

    /* grow the cache if necessary */
    if (n >= cap) {
        size_t newcap = (n + 1) * 2;
        if (newcap < cap * 2) newcap = cap * 2;
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* compute missing terms up to n (inclusive) */
    for (idx_t i = len; i <= n; ++i) {
        /* recurrence only valid for i >= 4, but i starts at len >= 4 here */
        seq_t term2 = cache[i - 2];
        seq_t term4 = cache[i - 4];
        cache[i] = (seq_t)56 * term2 - term4;
    }

    /* update length of known terms */
    len = n + 1;

    return cache[n];
}
