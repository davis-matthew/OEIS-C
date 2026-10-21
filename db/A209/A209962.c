/*
 * A209962  Number of (n+1) X 2 0..3 arrays containing all values 0..3 with
 *          every 2 X 2 subblock having one or two distinct values, and new
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A209962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 83*a(n-2) + 132*a(n-3) + 56*a(n-4) -
 *            88*a(n-5) - 32*a(n-6), for n > 6 Initial values: a(1)=0, a(2)=0,
 *            a(3)=9, a(4)=162, a(5)=1845, a(6)=17262 Offset: 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safety for n < 1 */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last_computed = 0;
    static bool init_done = false;

    /* initial cache allocation and population */
    if (!init_done) {
        cap = 8; /* enough for the first 6 terms */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < cap; ++i) cache[i] = 0;
        /* store initial terms (index 0 corresponds to a(1)) */
        cache[0] = 0;      /* a(1) */
        cache[1] = 0;      /* a(2) */
        cache[2] = 9;      /* a(3) */
        cache[3] = 162;    /* a(4) */
        cache[4] = 1845;   /* a(5) */
        cache[5] = 17262;  /* a(6) */
        last_computed = 6; /* highest term computed so far */
        init_done = true;
    }

    /* if requested term is within the cached range, return it */
    if (n <= last_computed)
        return cache[n-1];

    /* ensure the cache is large enough */
    size_t needed = (size_t)n;
    if (cap < needed) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap < needed) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = 16 * cache[i-2]
                  - 83 * cache[i-3]
                  + 132 * cache[i-4]
                  + 56 * cache[i-5]
                  - 88 * cache[i-6]
                  - 32 * cache[i-7];
        cache[i-1] = val;  /* store as a(i) */
    }
    last_computed = n;
    return cache[n-1];
}
