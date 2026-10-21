/*
 * A204379  Number of (n+2) X 8 0..1 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..1 introduced in
 *          row major order.
 * https://oeis.org/A204379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4) - a(n-5) (empirical)
 *            Initial values: a(1) = 286, a(2) = 128, a(3) = 139, a(4) = 153,
 *            a(5) = 176, a(6) = 217, a(7) = 283, a(8) = 388
 * Range:     u8 none, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t stored = 0;
    static size_t cap   = 0;

    /* Initialise cache with first 8 terms on first call */
    if (cache == NULL) {
        cap = 64;                               /* initial capacity */
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t)); /* index 0 unused */
        cache[1] = (seq_t)286;
        cache[2] = (seq_t)128;
        cache[3] = (seq_t)139;
        cache[4] = (seq_t)153;
        cache[5] = (seq_t)176;
        cache[6] = (seq_t)217;
        cache[7] = (seq_t)283;
        cache[8] = (seq_t)388;
        stored = 8;
    }

    /* If requested term already computed, return it */
    if (n <= stored)
        return cache[n];

    /* Grow cache if necessary */
    if (n > cap) {
        size_t new_cap = cap;
        while (new_cap < n)
            new_cap <<= 1;   /* double until large enough */
        cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute additional terms */
    for (idx_t i = stored + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3];
        cache[i] -= cache[i-4];
        cache[i] -= cache[i-5];
    }
    stored = n;
    return cache[n];
}
