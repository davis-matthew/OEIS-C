/*
 * A188557  Number of 6 X n binary arrays without the pattern 0 1 diagonally,
 *          vertically, antidiagonally or horizontally.
 * https://oeis.org/A188557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 122, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A188557: Number of 6×n binary arrays without a 0 1 pattern
   Recurrence (n ≥ 8):
      a(n) = 7·a(n‑1) – 21·a(n‑2) + 35·a(n‑3) – 35·a(n‑4)
             + 21·a(n‑5) – 7·a(n‑6) + a(n‑7)
      a(1) =  7, a(2) = 13, a(3) = 24, a(4) = 44, a(5) = 80,
             a(6) = 144, a(7) = 256
*/
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed values, 1‑based indexing internally */
    static seq_t *cache = NULL;   /* array of values a(1)…a(max_known) */
    static size_t cache_cap = 0;  /* currently allocated capacity */
    static idx_t known_max = 0;   /* largest n that has been computed */

    if (n == 0) return 0; /* out of bounds, but never used */

    /* If we have not yet initialized the base values, do so */
    if (known_max == 0) {
        const size_t init_cap = 8;                 /* at least 8 for recurrence */
        cache = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        if (!cache) abort();                      /* malloc failure */
        cache_cap = init_cap;
        static const seq_t base[7] = { (seq_t)7, (seq_t)13, (seq_t)24,
                                       (seq_t)44, (seq_t)80, (seq_t)144,
                                       (seq_t)256 };
        for (idx_t i = 1; i <= 7; ++i)
            cache[i-1] = base[i-1];
        known_max = 7;
    }

    /* Grow the cache if needed */
    if (known_max < n) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 8;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = known_max + 1; i <= n; ++i) {
        seq_t val =
            (seq_t)7   * cache[i-2]   -
            (seq_t)21  * cache[i-3]   +
            (seq_t)35  * cache[i-4]   -
            (seq_t)35  * cache[i-5]   +
            (seq_t)21  * cache[i-6]   -
            (seq_t)7   * cache[i-7]   +
            cache[i-8];
        cache[i-1] = val;
    }
    if (n > known_max) known_max = n;
    return cache[n-1];
}
