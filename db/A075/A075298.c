/*
 * A075298  Inverted (definition in A075193) generalized tribonacci numbers
 *          A001644.
 * https://oeis.org/A075298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1) - a(n-2) + a(n-3), a(0)=1, a(1)=1, a(2)=-5
 * Range:     i8 n <= 14, i16 n <= 31, i32 n <= 67, i64 n <= 141,
 *            i128 n <= 287, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache for arbitrary order of calls */
    static seq_t *cache      = NULL;
    static idx_t capacity    = 0;      /* allocated size */
    static idx_t cached_start = 0;     /* number of values currently cached (0 means uninitialised) */

    /* ---- allocate / grow cache so that index n can be accessed ---- */
    if (n >= capacity) {
        idx_t new_cap = (capacity == 0) ? 4 : capacity;
        while (new_cap <= n) new_cap <<= 1;   /* ensure new_cap > n */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* ---- initialise first three terms on first use ---- */
    if (cached_start == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)(-5);
        cached_start = 3;                     /* we now have terms 0..2 cached */
    }

    /* ---- if n is already cached, immediately return it ---- */
    if (n < cached_start)
        return cache[n];

    /* ---- compute missing terms up to n (inclusive) ---- */
    for (idx_t i = cached_start; i <= n; ++i) {
        cache[i] = -cache[i-1] - cache[i-2] + cache[i-3];
    }

    /* ---- update cached_start to reflect newly cached values ---- */
    cached_start = n + 1;     /* now cache[0 .. n] are valid */

    return cache[n];
}
