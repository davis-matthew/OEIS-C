/*
 * A216778  Number of derangements on n elements with an even number of
 *          cycles.
 * https://oeis.org/A216778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216778: Number of derangements on n elements with an even number of cycles.
 * Recurrence: a(0) = 1, a(1) = 0,
 *              a(n+1) = n * (a(n) + a(n-1) + n - 2)  for n >= 1.
 * Equivalent formula: a(n) = (A000166(n) - n + 1) / 2, where A000166 is the derangement numbers. */

#include <stdlib.h>

extern seq_t a(idx_t n);               /* forward declaration for clarity */

seq_t a(idx_t n)
{
    /* static cache holding previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Expand cache if necessary */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;                     /* indices 0 .. n inclusive */
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache)                            /* allocation failed */
            return (seq_t)0;                       /* graceful failure */

        /* Initialise base cases */
        if (new_len >= 1) new_cache[0] = (seq_t)1;
        if (new_len >= 2) new_cache[1] = (seq_t)0;

        /* Compute terms up to n using the recurrence */
        for (idx_t i = 2; i <= n; ++i) {
            seq_t temp = new_cache[i-1] + new_cache[i-2] + (seq_t)(i-1) - (seq_t)2;
            new_cache[i] = (seq_t)(i-1) * temp;
        }

        /* Free old cache and store new one */
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
