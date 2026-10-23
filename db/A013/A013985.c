/*
 * A013985  Expansion of 1/(1-x^2-x^3-x^4-x^5-x^6-x^7-x^8).
 * https://oeis.org/A013985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 49, u64 n <= 96, u128 n <= 191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1,
   a(1) = 0,
   a(n) = a(n-2) + a(n-3) + a(n-4) + a(n-5) + a(n-6) + a(n-7) + a(n-8)   for n >= 2
   This is the expansion of 1/(1−x²−x³−x⁴−x⁵−x⁶−x⁷−x⁸). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;   /* stores a(0)..a(max_n) */
    static idx_t max_n = 0;       /* highest index cached */

    /* Grow cache if needed */
    if (cache == NULL || max_n < n) {
        idx_t new_cap = n + 1;

        /* Allocate new buffer */
        seq_t *new_cache = (seq_t*)malloc((size_t)new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* Allocation failure, unlikely */

        if (cache == NULL) {
            /* First allocation: initialize all to 0 and then compute values */
            for (idx_t i = 0; i < new_cap; ++i) new_cache[i] = (seq_t)0;
            new_cache[0] = (seq_t)1; /* a(0) */
            for (idx_t i = 1; i < new_cap; ++i) {
                seq_t sum = (seq_t)0;
                for (idx_t k = 2; k <= 8; ++k) {
                    if (i >= k) sum += new_cache[i - k];
                }
                new_cache[i] = sum;
            }
        } else {
            /* Copy existing values and compute new ones */
            for (idx_t i = 0; i <= max_n; ++i) new_cache[i] = cache[i];
            for (idx_t i = max_n + 1; i < new_cap; ++i) {
                seq_t sum = (seq_t)0;
                for (idx_t k = 2; k <= 8; ++k) {
                    if (i >= k) sum += new_cache[i - k];
                }
                new_cache[i] = sum;
            }
        }

        /* Replace old cache */
        free(cache);
        cache = new_cache;
        max_n = n;
    }

    return cache[n];
}
