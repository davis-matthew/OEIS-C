/*
 * A220634  Number of ways to reciprocally link elements of an 4 X n array
 *          either to themselves or to exactly two horizontal or antidiagonal
 *          neighbors.
 * https://oeis.org/A220634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(3)=5, a(4)=37, a(5)=257, a(6)=1665,
 *            a(7)=10695, a(8)=68637, a(9)=440509; a(n) = 9*a(n-1) - 16*a(n-2)
 *            - 6*a(n-3) + 15*a(n-4) + a(n-5) - 2*a(n-6) (n>9)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Returns the n-th term of OEIS A220634 (n >= 1). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety guard – unused */

    /* Static cache for memoisation. cache[i] holds a(i+1). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of cached entries */

    /* Known initial values a(1) .. a(9). */
    static const seq_t init[9] = {
        (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)37,
        (seq_t)257, (seq_t)1665, (seq_t)10695,
        (seq_t)68637, (seq_t)440509
    };

    size_t needed = (size_t)n;
    if (n < 10 && needed < 9) needed = 9;   /* ensure space for the 9 init terms */

    /* Initialise cache on first call. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * needed);
        if (!cache) abort();
        for (size_t i = 0; i < 9; ++i) cache[i] = init[i];
        cache_len = 9;
    }
    /* Grow cache if required. */
    else if (cache_len < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
        if (!tmp) abort();
        cache = tmp;
        /* Fill newly allocated slots. */
        for (size_t i = cache_len; i < needed; ++i) {
            /* recurrence only valid for i >= 9 (i.e. n = i+1 >= 10) */
            if (i >= 9) {
                seq_t t = (seq_t)9 * cache[i-1];
                t -= (seq_t)16 * cache[i-2];
                t -= (seq_t)6  * cache[i-3];
                t += (seq_t)15 * cache[i-4];
                t +=        cache[i-5];
                t -= (seq_t)2  * cache[i-6];
                cache[i] = t;
            } else {
                cache[i] = init[i];
            }
        }
        cache_len = needed;
    }

    return cache[n-1];
}
