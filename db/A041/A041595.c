/*
 * A041595  Denominators of continued fraction convergents to sqrt(315).
 * https://oeis.org/A041595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 142*a(n-4) - a(n-8) for n > 7 Initial values: a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=4, a(4)=139, a(5)=143, a(6)=425, a(7)=568
 *            This implements the recurrence with lazy memoisation, allowing
 *            calls in any order
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static buffer for memoised terms */
    static seq_t *cache     = NULL;
    static size_t capacity  = 0;          /* allocated size       */
    static size_t max_computed = 0;       /* highest index computed */

    /* initialise the first 8 terms once */
    if (cache == NULL) {
        capacity = 8;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) abort();    /* cannot proceed without memory */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)139;
        cache[5] = (seq_t)143;
        cache[6] = (seq_t)425;
        cache[7] = (seq_t)568;
        max_computed = 7;
    }

    /* if the term is already known, return it */
    if (n <= max_computed) return cache[n];

    /* grow the cache to hold all indices up to n */
    if ((size_t)n >= capacity) {
        size_t new_cap = capacity ? capacity : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();                     /* out of memory */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute any missing terms using the recurrence */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)142 * cache[i - 4] - cache[i - 8];
    }
    max_computed = (size_t)n;

    return cache[n];
}
