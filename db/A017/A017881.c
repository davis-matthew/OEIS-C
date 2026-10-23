/*
 * A017881  Expansion of 1/(1 - x^9 - x^10 - x^11 - x^12 - x^13 - x^14).
 * https://oeis.org/A017881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 84, u32 n <= 154, u64 n <= 294,
 *            u128 n <= 573, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A017881
 *  Expansion of 1/(1 - x^9 - x^10 - x^11 - x^12 - x^13 - x^14)
 *
 *  Recurrence:
 *      a(0) = 1
 *      a(1) = … = a(8)   = 0
 *      a(9)  = … = a(14) = 1
 *      a(n) (n≥15) = a(n-9) + a(n-10) + a(n-11) + a(n-12)
 *                      + a(n-13) + a(n-14)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache      = NULL;  /* array of seq_t */
    static idx_t cache_cap   = 0;     /* allocated size (in elements) */
    static idx_t last_computed = 0;  /* highest index whose value is set */

    /* Allocate or grow cache if needed */
    if (( idx_t )cache_cap <= n) {
        idx_t new_cap = n + 1;
        /* C++ needs an explicit cast for realloc */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failure */
        cache = new_cache;

        /* Initialize newly allocated entries */
        for (idx_t i = cache_cap; i < new_cap; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i < 9)
                cache[i] = (seq_t)0;
            else if (i < 15)
                cache[i] = (seq_t)1;
            else
                cache[i] = (seq_t)0; /* placeholder for later computation */
        }
        cache_cap = new_cap;
    }

    /* If we already computed up to n, just return the value */
    if (last_computed >= n)
        return cache[n];

    /* Compute values from the next index after the last computed one up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i < 9) {
            cache[i] = (seq_t)0;   /* a(1)..a(8) = 0 */
        } else if (i < 15) {
            cache[i] = (seq_t)1;   /* a(9)..a(14) = 1 */
        } else {
            cache[i] = cache[i-9] + cache[i-10] + cache[i-11]
                     + cache[i-12] + cache[i-13] + cache[i-14];
        }
    }

    last_computed = n;
    return cache[n];
}
