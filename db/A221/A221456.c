/*
 * A221456  Number of 0..5 arrays of length n with each element unequal to at
 *          least one neighbor, with new values introduced in 0..5 order.
 * https://oeis.org/A221456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for sequence A221456:
   a(n) = 11*a(n-1) - 30*a(n-2) - 21*a(n-3) + 112*a(n-4) + 63*a(n-5)
          - 119*a(n-6) - 120*a(n-7) - 30*a(n-8);
   Initial values: a(1)=0, a(2)=1, a(3)=2, a(4)=7, a(5)=25,
   a(6)=102, a(7)=455, a(8)=2192. */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* cache[0] unused, cache[1..] holds a(1..) */
    static idx_t cached_n = 0;       /* highest n currently cached     */

    if (n < 1) return (seq_t)0;       /* undefined for n<1, return 0 */

    /* Return cached value if already computed. */
    if (n <= cached_n) return cache[n];

    /* Grow cache to size n. */
    idx_t new_n = n;
    seq_t *new_cache;

#if defined(__cplusplus)
    new_cache = new seq_t[new_n + 1];
    if (cache) {
        for (idx_t i = 0; i <= cached_n; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
#else
    new_cache = (seq_t *)realloc(cache, (new_n + 1) * sizeof(seq_t));
    if (!new_cache) abort();          /* out of memory */
#endif

    cache = new_cache;

    /* Initial values for 1..8 (a(1)..a(8)). */
    static const seq_t init_vals[9] = {0, 0, 1, 2, 7, 25, 102, 455, 2192};

    /* Compute new terms. */
    for (idx_t i = cached_n + 1; i <= new_n; ++i) {
        if (i <= 8) {
            cache[i] = init_vals[i];
        } else {
            cache[i] = 11 * cache[i-1]
                     - 30 * cache[i-2]
                     - 21 * cache[i-3]
                     + 112 * cache[i-4]
                     + 63  * cache[i-5]
                     - 119 * cache[i-6]
                     - 120 * cache[i-7]
                     - 30  * cache[i-8];
        }
    }

    cached_n = new_n;
    return cache[n];
}
