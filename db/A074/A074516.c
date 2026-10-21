/*
 * A074516  a(n) = 1^n + 5^n + 6^n.
 * https://oeis.org/A074516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 5^n + 6^n. Recurrence: a(0) = 3, a(1) = 12, a(n) =
 *            11*a(n-1) - 30*a(n-2) + 20 for n > 1. The recurrence is derived
 *            from the generating function 1/(1-x) + 1/(1-5x) + 1/(1-6x) and
 *            holds for all n >= 0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: store already computed terms. */
    static seq_t *cache = NULL;          /* array of computed terms      */
    static idx_t cache_size = 0;         /* number of elements stored   */

    /* Initialise cache with first two terms. */
    if (cache_size == 0) {
        cache_size   = 2;
        cache        = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0]     = (seq_t)3;          /* a(0) */
        cache[1]     = (seq_t)12;         /* a(1) */
    }

    /* Extend cache if needed, computing new terms with the recurrence. */
    if (n >= cache_size) {
        /* Grow the array to accommodate indices up to n. */
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        cache = tmp;          /* assume allocation succeeds */

        /* Compute missing terms. */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)3;
            } else if (i == 1) {
                val = (seq_t)12;
            } else {
                /* a(i) = 11*a(i-1) - 30*a(i-2) + 20 */
                val = ((seq_t)11) * cache[i-1];
                val = val - ((seq_t)30) * cache[i-2];
                val = val + ((seq_t)20);
            }
            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
