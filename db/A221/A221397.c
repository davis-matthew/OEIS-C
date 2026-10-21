/*
 * A221397  Number of n X 3 arrays of occupancy after each element moves to
 *          some horizontal, diagonal or antidiagonal neighbor, with no
 *          2-loops and with no occupancy greater than 2.
 * https://oeis.org/A221397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2) - 2*a(n-3) with initial values
 *            a(1)=0, a(2)=6, a(3)=30
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* pointer to computed terms */
static idx_t cache_cap = 0;       /* allocated capacity (max index allowed) */
static idx_t computed = 0;        /* largest n for which cache[n] is valid */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */

    /* first call: allocate storage and set known base terms */
    if (cache == NULL) {
        idx_t init_cap = n > 3 ? n : 3;     /* at least enough for first 3 terms */
        cache_cap = init_cap;
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)30;
        computed = 3;
    }

    /* enlarge storage if needed */
    if (n > cache_cap) {
        idx_t new_cap = n;
        if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)6 * cache[i-1]
                     - (seq_t)6 * cache[i-2]
                     - (seq_t)2 * cache[i-3];
        }
        computed = n;
    }

    return cache[n];
}
