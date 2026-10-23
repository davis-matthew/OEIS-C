/*
 * A041151  Denominators of continued fraction convergents to sqrt(85).
 * https://oeis.org/A041151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 756*a(n-5) + a(n-10) for n ≥ 10 Initial values: a(0) = 1
 *            , a(1) = 4 , a(2) = 5 , a(3) = 9 , a(4) = 41 , a(5) = 747 , a(6)
 *            = 3029 , a(7) = 3776 , a(8) = 6805 , a(9) = 30996
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* Base values for n = 0 … 9 */
    static const seq_t base[10] = {
        (seq_t)1, (seq_t)4, (seq_t)5, (seq_t)9, (seq_t)41,
        (seq_t)747, (seq_t)3029, (seq_t)3776, (seq_t)6805, (seq_t)30996
    };

    /* Cache of computed terms */
    static seq_t *cache   = NULL;   /* array of computed a(i) */
    static idx_t cache_len = 0;     /* current size (max index + 1) */

    /* Handle small indices directly */
    if (n < 10)
        return base[(size_t)n];

    /* First allocation: store the 10 base values */
    if (!cache) {
        cache_len = 10;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure */

        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = base[(size_t)i];
    }

    /* Grow the cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */

        /* copy already computed values */
        for (idx_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];

        /* free old storage */
        free(cache);
        cache = tmp;
        cache_len = new_len;

        /* compute missing terms using the recurrence */
        for (idx_t i = 10; i <= n; ++i) {
            cache[i] = (seq_t)756 * cache[i - 5] + cache[i - 10];
        }
    }

    return cache[(size_t)n];
}
