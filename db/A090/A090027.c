/*
 * A090027  Number of distinct lines through the origin in 5-dimensional cube
 *          of side length n.
 * https://oeis.org/A090027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^5 - 1 - Σ_{j=2}^{n+1} a(⌊n/j⌋)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n‑th term of OEIS A090027.
 * The function is memoised; successive calls may be in any order. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* table of previously computed terms   */
    static idx_t cache_size = 0;     /* number of entries currently stored   */

    /* Allocate (or enlarge) the cache to hold values up to n */
    if (n + 1 > cache_size) {
        size_t new_len = (size_t)(n + 1);
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!cache) return 0;          /* should never happen in tests   */
            cache[0] = 0;                  /* a(0) = 0                        */
            cache_size = new_len;
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) return 0;
            cache = tmp;
            cache_size = new_len;
        }

        /* Compute all missing values from the previous cache_size up to n */
        for (idx_t i = 1; i <= n; ++i) {
            seq_t val = (seq_t)(i + 1);                 /* (i+1)           */
            val = val * val * val * val * val;          /* (i+1)^5         */
            val = val - 1;                              /* (i+1)^5 - 1     */

            for (idx_t j = 2; j <= i + 1; ++j) {        /* sum over j       */
                idx_t k = i / j;                       /* floor(i/j)       */
                val -= cache[k];                       /* subtract a(k)    */
            }
            cache[i] = val;
        }
    }
    return cache[n];
}
