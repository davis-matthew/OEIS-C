/*
 * A140428  a(n) = A000045(n) + A113405(n).
 * https://oeis.org/A140428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + 3*a(n-4) - a(n-5) -
 *            2*a(n-6) with initial values a(0)=0, a(1)=1, a(2)=1, a(3)=3,
 *            a(4)=5, a(5)=9
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static const seq_t base[6] = {0, 1, 1, 3, 5, 9};

    /* Return the first six terms directly. */
    if (n < 6)
        return base[n];

    static seq_t *cache = NULL;   /* dynamically grown array of computed terms */
    static size_t cacheCap = 0;   /* allocated capacity */
    static size_t cacheComp = 0;  /* number of terms actually computed */

    /* Grow the cache if necessary. */
    if (cacheCap <= (size_t)n) {
        size_t newCap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (!tmp)
            return 0;  /* allocation failure – return 0 as a safe fallback */

        if (!cache) {
            /* First allocation: initialise the first six terms. */
            for (size_t i = 0; i < 6; ++i)
                tmp[i] = base[i];
            cacheComp = 6;
        }
        cache = tmp;
        cacheCap = newCap;
    }

    /* Compute any terms from cacheComp up to n. */
    for (size_t i = cacheComp; i <= (size_t)n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];
        cache[i] = 3 * a1 - a2 - 3 * a3 + 3 * a4 - a5 - 2 * a6;
    }
    cacheComp = (size_t)n + 1;

    return cache[n];
}
