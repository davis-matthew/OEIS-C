/*
 * A200250  Number of 0..7 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its previous elements modulo 8.
 * https://oeis.org/A200250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 9*a(n-2) + 24*a(n-3) + 36*a(n-4) + 35*a(n-5) +
 *            21*a(n-6) + 7*a(n-7) + a(n-8) init: a(1) = 8, a(2) = 36, a(3) =
 *            164, a(4) = 750, a(5) = 3434, a(6) = 15724, a(7) = 71970, a(8) =
 *            329455
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage: function name and argument types are defined by the build system */
seq_t a(idx_t n)
{
    /* static cache indexed from 1; cache[0] is unused */
    static seq_t *cache      = NULL;
    static idx_t  cache_cap  = 0;   /* maximum index that can be stored (cache_cap >= cache_len) */
    static idx_t  cache_len  = 0;   /* highest index currently filled    */

    /* The sequence only has meaning for n >= 1.
       If called with n == 0 just return 0; the exact value for n==0
       is irrelevant for the OEIS offset 1.                       */
    if (n == 0) return (seq_t)0;

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        /* Minimum capacity: |a(1) … a(8)| */
        cache_cap = 8;
        cache_len = 8;
        cache = (seq_t*)malloc((cache_cap + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure */

        cache[1] = (seq_t)8;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)164;
        cache[4] = (seq_t)750;
        cache[5] = (seq_t)3434;
        cache[6] = (seq_t)15724;
        cache[7] = (seq_t)71970;
        cache[8] = (seq_t)329455;
    }

    /* Return a cached value if already computed */
    if (n <= cache_len) return cache[n];

    /* Grow the cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms up to n using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* For i >= 9 the recurrence holds; for i<=8 we already have the values */
        cache[i] =
            cache[i-1]                            +
            (seq_t)9  * cache[i-2]                +
            (seq_t)24 * cache[i-3]                +
            (seq_t)36 * cache[i-4]                +
            (seq_t)35 * cache[i-5]                +
            (seq_t)21 * cache[i-6]                +
            (seq_t)7  * cache[i-7]                +
            cache[i-8];
    }
    cache_len = n;
    return cache[n];
}
