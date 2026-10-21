/*
 * A205348  Number of (n+1)X3 0..3 arrays with no 2X2 subblock having the same
 *          number of equal edges as its horizontal or vertical neighbors, and
 *          new values 0..3 introduced in row major order.
 * https://oeis.org/A205348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) +108*a(n-2) +739*a(n-3) +534*a(n-4) -2929*a(n-5)
 *            +8365*a(n-6) +5012*a(n-7) -23096*a(n-8) -13776*a(n-9)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence with initial values:
   a(1)=110, a(2)=1750, a(3)=30904, a(4)=544616, a(5)=9559986,
   a(6)=168343322, a(7)=2962323362, a(8)=52122083398, a(9)=917209854720 */

#include <stdlib.h>

static const seq_t init_vals[9] = {
    (seq_t)110,
    (seq_t)1750,
    (seq_t)30904,
    (seq_t)544616,
    (seq_t)9559986,
    (seq_t)168343322,
    (seq_t)2962323362,
    (seq_t)52122083398,
    (seq_t)917209854720
};

seq_t a(idx_t n) {
    /*  Cache of already computed values; indices 0..n-1 correspond to a(1)..a(n) */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* number of cached entries (n)   */

    /* For n < 10 we can return the known initial values directly */
    if (n < 10) return init_vals[n-1];

    /* Ensure the cache is large enough for indices up to n-1 */
    if (n > cap) {
        idx_t new_cap = n;
        if (new_cap < 10) new_cap = 10;   /* at least store first 9 values */

        /* Reallocate the cache */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed */
            exit(EXIT_FAILURE);
        }
        cache = tmp;

        /* Initialize the first 9 entries (a(1)..a(9)) */
        if (cap < 9) {
            for (idx_t i = cap; i < 9; ++i) {
                cache[i] = init_vals[i];
            }
        }

        /* Compute missing entries using the recurrence */
        for (idx_t i = (cap > 9 ? cap : 9); i < new_cap; ++i) {
            /* positve part of the recurrence */
            seq_t pos = (seq_t)9   * cache[i-1]
                      + (seq_t)108 * cache[i-2]
                      + (seq_t)739 * cache[i-3]
                      + (seq_t)534 * cache[i-4]
                      + (seq_t)8365 * cache[i-6]
                      + (seq_t)5012 * cache[i-7];

            /* negative part of the recurrence */
            seq_t neg = (seq_t)2929 * cache[i-5]
                      + (seq_t)23096 * cache[i-8]
                      + (seq_t)13776 * cache[i-9];

            /* All terms are positive; pos is guaranteed >= neg for a correct recurrence */
            cache[i] = pos - neg;
        }

        cap = new_cap;
    }

    /* Return the cached value */
    return cache[n-1];
}
