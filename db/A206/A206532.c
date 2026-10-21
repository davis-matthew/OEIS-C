/*
 * A206532  a(n) = (2(n+1)(2n+1)-1) * a(n-1) + 2n(2n-1) * a(n-2), a(0) = 1,
 *          a(1) = 11.
 * https://oeis.org/A206532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2(n+1)(2n+1)-1) * a(n-1) + 2n(2n-1) * a(n-2), a(0) = 1,
 *            a(1) = 11
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t len = 0;          /* Number of entries that have been computed */

    /* Ensure the cache is large enough for indices 0..n */
    size_t needed = (size_t)n + 1;
    if (!cache || len < needed) {
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* Allocation failed */
    }

    /* Initialise the first two entries if they have not been set yet */
    if (len == 0) {
        cache[0] = (seq_t)1;
        if (needed > 1) cache[1] = (seq_t)11;
        len = (needed > 1) ? 2 : 1;
    } else if (len == 1) {
        cache[1] = (seq_t)11;
        len = 2;
    }

    /* Compute missing terms in order */
    for (size_t i = len; i <= n; ++i) {
        idx_t ii = (idx_t)i;  /* Current index n */

        /* factor1 = 2*(n+1)*(2*n+1) - 1   */
        seq_t factor1 = ((seq_t)2 * ((seq_t)(ii + 1)) *
                         ((seq_t)(2 * ii + 1)) - (seq_t)1);
        /* factor2 = 2*n*(2*n-1) */
        seq_t factor2 = ((seq_t)2 * ((seq_t)ii) *
                         ((seq_t)(2 * ii - 1)));

        cache[i] = factor1 * cache[i - 1] + factor2 * cache[i - 2];
    }
    len = needed;   /* All entries up to n are now cached */
    return cache[n];
}
