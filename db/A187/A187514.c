/*
 * A187514  Number of 9-step S, E, and NW-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n>10, with initial
 *            values a(1)=0, a(2)=0, a(3)=6, a(4)=450, a(5)=2872, a(6)=8522,
 *            a(7)=17564, a(8)=29834, a(9)=45255, a(10)=63814
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for already computed terms.  Index 0 is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Handle call below the offset.  Behaviour is arbitrary. */
    if (n < 1) return 0;

    /* Initialisation: allocate storage and fill the first ten terms. */
    if (cache_size == 0) {
        cache_size = (n > 10) ? n : 10;
        cache = (seq_t*)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */

        static const seq_t base[10] = {
            0, 0, 6,          /* a(1) .. a(3)   */
            450, 2872, 8522,  /* a(4) .. a(6)   */
            17564, 29834,     /* a(7) .. a(8)   */
            45255, 63814      /* a(9) .. a(10)  */
        };

        for (idx_t i = 1; i <= 10; ++i)
            cache[i] = base[i - 1];
    }

    /* If the requested term is already in the cache, return it. */
    if (n <= cache_size)
        return cache[n];

    /* Extend the cache to accommodate the new value(s). */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return 0;   /* allocation failure */
    cache = tmp;

    /* Compute new terms using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i - 1]
                 - (seq_t)3 * cache[i - 2]
                 + cache[i - 3];
    }
    cache_size = n;

    return cache[n];
}
