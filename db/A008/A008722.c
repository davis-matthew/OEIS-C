/*
 * A008722  Molien series for 3-dimensional group [2,9] = *229.
 * https://oeis.org/A008722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) - a(n-4) + a(n-9) - 2*a(n-11) + a(n-13) (initial
 *            values: a(0)=1, a(1)=0, a(2)=2, a(3)=0, a(4)=3, a(5)=0, a(6)=4,
 *            a(7)=0, a(8)=5, a(9)=1, a(10)=6, a(11)=2, a(12)=7, a(13)=3)
 * Range:     u8 n <= 125, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_len = 0;     /* number of terms currently stored (next free index) */
static size_t cache_cap = 0;     /* allocated capacity */

/* initial values up to n = 13 */
static const seq_t base_vals[14] = {
    1, 0, 2, 0, 3, 0, 4, 0, 5, 1, 6, 2, 7, 3
};

seq_t a(idx_t n)
{
    /* Expand the cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < n + 1)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            exit(EXIT_FAILURE);  /* allocation failure */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n + 1 > cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 13) {          /* use the stored initial values */
                cache[i] = base_vals[i];
            } else {                /* recurrence for i >= 14 */
                cache[i] = (2 * cache[i - 2]) - cache[i - 4]
                           + cache[i - 9] - (2 * cache[i - 11])
                           + cache[i - 13];
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
