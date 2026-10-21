/*
 * A238829  a(n) = A238823(n) - A238826(n).
 * https://oeis.org/A238829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A238823(n) - A238826(n) Generating function:
 *            -x*(x-1)*(2*x^5+x^4+x^3-2*x^2-x+1) / (
 *            1-3*x+4*x^3-x^4-x^5-3*x^6+x^7 ) Equivalent linear recurrence for
 *            n >= 8: a(n) = 3*a(n-1) - 4*a(n-3) + a(n-4) + a(n-5) + 3*a(n-6)
 *            - a(n-7) Initial terms (offset 1): a(1)=1, a(2)=1, a(3)=2,
 *            a(4)=5, a(5)=12, a...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for the unused 0‑th index (might be called by the test). */
    if (n == 0) return (seq_t)0;

    /* Static cache for computed values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated size */
    static idx_t  computed_up_to = 0;     /* highest index computed */

    /* Ensure the cache can hold indices up to n. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = (needed > 8 ? needed : 8);   /* at least enough for a[0..7] */
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure */

        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];

        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise the first eight terms (index 0 is dummy 0). */
    if (computed_up_to < 7) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)12;
        cache[6] = (seq_t)31;
        cache[7] = (seq_t)77;
        computed_up_to = 7;
    }

    /* Compute any missing values up to n. */
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            if (i < 8) /* already set */ continue;

            /* Compute
               s = 3*a(i-1) + a(i-4) + a(i-5) + 3*a(i-6)
               t = 4*a(i-3) + a(i-7)
               a(i) = s - t                                     */
            seq_t s = (seq_t)3 * cache[i-1] + cache[i-4]
                    + cache[i-5] + (seq_t)3 * cache[i-6];
            seq_t t = (seq_t)4 * cache[i-3] + cache[i-7];
            cache[i] = s - t;   /* s >= t in the sequence, so no underflow */
        }
        computed_up_to = n;
    }

    return cache[n];
}
