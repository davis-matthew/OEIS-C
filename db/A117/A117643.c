/*
 * A117643  a(n) = n*(a(n-1)-1) starting with a(0)=3.
 * https://oeis.org/A117643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 3, a(n) = n * (a(n-1) - 1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements currently allocated */
    static idx_t computed   = 0;   /* highest index that has been computed */

    /* Make sure we have enough space for indices 0 .. n */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) exit(1);            /* allocation failure */
        cache = tmp;
        cache_size = n + 1;
    }

    /* Initialise the base case once */
    if (computed == 0) {
        cache[0] = (seq_t)3U;
    }

    /* Compute any uncalculated terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)i * (cache[i - 1] - (seq_t)1U);
    }
    computed = n;

    return cache[n];
}
