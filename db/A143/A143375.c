/*
 * A143375  Expansion of x/(1 - x^2 - 2*x^5 - x^8 - x^10 - x^12).
 * https://oeis.org/A143375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 38, u32 n <= 70, u64 n <= 136,
 *            u128 n <= 268, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + 2*a(n-5) + a(n-8) + a(n-10) + a(n-12)
   (OEIS A143375)
   initial terms: a(1)=1, a(2)=0, a(3)=1, a(4)=0, a(5)=1, a(6)=2,
   a(7)=1, a(8)=4, a(9)=2, a(10)=6, a(11)=8, a(12)=8
*/

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;

    static const seq_t init[12] = {
        (seq_t)1, /* a(1) */
        (seq_t)0, /* a(2) */
        (seq_t)1, /* a(3) */
        (seq_t)0, /* a(4) */
        (seq_t)1, /* a(5) */
        (seq_t)2, /* a(6) */
        (seq_t)1, /* a(7) */
        (seq_t)4, /* a(8) */
        (seq_t)2, /* a(9) */
        (seq_t)6, /* a(10) */
        (seq_t)8, /* a(11) */
        (seq_t)8  /* a(12) */
    };

    /* Return known initial terms directly */
    if (n <= 12)
        return init[n - 1];

    /* Ensure cache large enough for index n */
    if (!cache || cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0; /* allocation failed, unlikely in tests */

        if (cache) {
            memcpy(new_cache, cache, (size_t)(max_computed + 1) * sizeof(seq_t));
            free(cache);
        } else {
            memset(new_cache, 0, new_cap * sizeof(seq_t));
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize base values if not yet done */
    if (max_computed < 12) {
        for (idx_t i = 1; i <= 12; ++i)
            cache[i] = init[i - 1];
        max_computed = 12;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] =
            cache[i - 2] + 2 * cache[i - 5] + cache[i - 8] +
            cache[i - 10] + cache[i - 12];
    }
    max_computed = n;

    return cache[n];
}
