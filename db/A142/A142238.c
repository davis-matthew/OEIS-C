/*
 * A142238  Numerators of continued fraction convergents to sqrt(3/2).
 * https://oeis.org/A142238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=11, a(3)=49, a(n)=10*a(n-2)-a(n-4) for n>=4
 *            (numerators of continued‑fraction convergents to sqrt(3/2)) This
 *            recurrence generates the OIES sequence A142238
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms             */
    static size_t cached = 0;     /* number of terms currently cached     */

    /* ensure the cache can hold the requested term */
    size_t needed = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – unlikely in tests */
        cached = 0;
    } else if (needed > cached) {
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cached = 0;  /* will rebuild from scratch */
    }

    /* initialise first four terms if not yet done */
    if (cached == 0) {
        cache[0] = (seq_t)1;
        if (needed > 1) cache[1] = (seq_t)5;
        if (needed > 2) cache[2] = (seq_t)11;
        if (needed > 3) cache[3] = (seq_t)49;
        cached = 4;
    }

    /* fill the cache up to the required term */
    for (size_t i = cached; i < needed; ++i) {
        /* recurrence: a(i) = 10*a(i-2) - a(i-4)  (valid for i>=4) */
        cache[i] = (seq_t)(10 * cache[i - 2]) - cache[i - 4];
    }
    cached = needed;

    return cache[(size_t)n];
}
