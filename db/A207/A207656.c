/*
 * A207656  Number of n X 3 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - a(n-3) for n>4; a(1)=6, a(2)=36,
 *            a(3)=84, a(4)=198. The function memoises results using a simple
 *            recurrence and handles arbitrary‑precision seq_t by using only
 *            +, -, and assignments
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cached values: indices start at 1; cache[0] is unused */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* allocated length (in seq_t elements) */
    static idx_t max_stored = 0;  /* largest n for which cache[n] is valid */

    /* ensure capacity for indices up to n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 5) new_cap = 5;                /* room for the first 4 terms */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (cache == NULL) return 0;             /* allocation failed */
            cap = new_cap;
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (cache == NULL) return 0;
            cap = new_cap;
        }
    }

    /* initialise the first four terms once */
    if (max_stored == 0 && cap >= 5) {
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)84;
        cache[4] = (seq_t)198;
        max_stored = 4;
    }

    /* already computed? */
    if (n <= max_stored) return cache[n];

    /* compute missing terms up to n via the recurrence */
    for (idx_t i = max_stored + 1; i <= n; ++i) {
        /* compute 3*a(i-1) using only addition to avoid 64‑bit overflow */
        seq_t t = cache[i - 1];
        t += cache[i - 1];
        t += cache[i - 1];
        t -= cache[i - 2];
        t -= cache[i - 3];
        cache[i] = t;
    }

    max_stored = n;
    return cache[n];
}
