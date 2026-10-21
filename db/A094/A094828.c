/*
 * A094828  Number of (s(0), s(1), ..., s(2n)) such that 0 < s(i) < 9 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n, s(0) = 1, s(2n) = 5.
 * https://oeis.org/A094828
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 15*a(n-2) + 10*a(n-3) - a(n-4) Initial values:
 *            a(2)=1, a(3)=5, a(4)=20, a(5)=75 Offset: 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 1;   /* highest index with a defined value */

    /* Allocate or grow cache to fit index 'n' */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – return 0 as a safe default */
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i) cache[i] = 0;
        cache_size = new_size;

        /* Initialize the initial values that are within bounds */
        if (cache_size > 2) cache[2] = 1;
        if (cache_size > 3) cache[3] = 5;
        if (cache_size > 4) cache[4] = 20;
        if (cache_size > 5) cache[5] = 75;
    }

    /* Compute terms up to 'n' if not already done */
    if (n > last_computed) {
        idx_t start = last_computed + 1;
        if (start < 6) start = 6;           /* recurrence valid from n = 6 */
        for (idx_t i = start; i <= n; ++i) {
            seq_t left  = 7 * cache[i-1] + 10 * cache[i-3];
            seq_t right = 15 * cache[i-2] + cache[i-4];
            cache[i] = left - right;
        }
        last_computed = n;
    }

    return cache[n];
}
