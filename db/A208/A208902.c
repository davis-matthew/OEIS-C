/*
 * A208902  The sum over all bitstrings b of length n of the number of runs in
 *          b not immediately followed by a longer run.
 * https://oeis.org/A208902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) - 6*a(n-3) + 16*a(n-4) - 8*a(n-5) for
 *            n ≥ 6 with initial values a(1)=2, a(2)=6, a(3)=14, a(4)=34,
 *            a(5)=78
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Base cases for n = 1 … 5 */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;  /* number of terms currently stored */

    if (n == 0) return 0;      /* undefined value – return 0 */

    size_t need = (size_t)n;   /* requested number of terms */

    /* Initialise base cache if first call */
    if (cache_size == 0) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure */
        cache[0] = 2;          /* a(1) */
        cache[1] = 6;          /* a(2) */
        cache[2] = 14;         /* a(3) */
        cache[3] = 34;         /* a(4) */
        cache[4] = 78;         /* a(5) */
    }

    /* Grow cache if further terms are needed */
    if (need > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failure */
        cache = new_cache;

        /* Compute missing terms using the recurrence */
        for (size_t i = cache_size; i < need; ++i) {
            if (i < 5) continue;  /* should not happen */
            cache[i] = 5 * cache[i - 1]
                     - 6 * cache[i - 2]
                     - 6 * cache[i - 3]
                     + 16 * cache[i - 4]
                     - 8 * cache[i - 5];
        }
        cache_size = need;
    }

    return cache[need - 1];
}
