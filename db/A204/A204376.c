/*
 * A204376  Number of (n+2) X 5 0..1 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..1 introduced in
 *          row major order.
 * https://oeis.org/A204376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-4)-a(n-5) for n>8 Initial
 *            values: a(1)=78, a(2)=54, a(3)=61, a(4)=75, a(5)=98, a(6)=139,
 *            a(7)=205, a(8)=310
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* Handle the offset value 0, though it is not part of the sequence. */
    if (n == 0)
        return (seq_t)0;

    /* Static storage for the computed sequence. */
    static seq_t *cache          = NULL;
    static idx_t  cache_size     = 0;     /* allocated array size   */
    static idx_t  max_cached     = 0;     /* highest index computed */

    /* Allocate and initialise the cache the first time it is needed. */
    if (cache == NULL) {
        /* Allocate space for indices 0..8 (size 9) */
        cache_size = 9;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache)
            abort();

        /* Initialise the known values (index 0 is unused). */
        cache[1] = (seq_t)78;
        cache[2] = (seq_t)54;
        cache[3] = (seq_t)61;
        cache[4] = (seq_t)75;
        cache[5] = (seq_t)98;
        cache[6] = (seq_t)139;
        cache[7] = (seq_t)205;
        cache[8] = (seq_t)310;
        max_cached = 8;
    }

    /* Grow the cache if the requested index is outside the current size. */
    if (n >= cache_size) {
        idx_t new_size = (n + 1) * 2;     /* overshoot to amortise */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_size * sizeof(seq_t));
        if (!tmp && new_size > 0)
            abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* If the value is already computed, return it. */
    if (n <= max_cached)
        return cache[n];

    /* Compute the missing terms up to n using the recurrence. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* All indices i-1 ... i-5 are valid for i >= 9. */
        seq_t sum1 = cache[i-1] + cache[i-2] + cache[i-3];
        seq_t sum2 = cache[i-4] + cache[i-5];
        cache[i]   = sum1 - sum2;
    }
    max_cached = n;
    return cache[n];
}
