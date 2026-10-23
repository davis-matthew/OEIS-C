/*
 * A118005  a(n) = ((-1)^n*5^(n+1) + 9^(n+1))/14.
 * https://oeis.org/A118005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((-1)^n*5^(n+1) + 9^(n+1))/14 Recurrence (fast): a(0)=1,
 *            a(1)=4, a(n)=4*a(n-1)+45*a(n-2) (n≥2) The function memoizes
 *            computed values so repeated calls are O(1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoization buffer */
static idx_t cache_size = 0;  /* number of entries in cache */

/* Compute a(n) for the sequence A118005 */
seq_t a(idx_t n)
{
    /* Base cases handled directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* Allocate or grow cache as needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;               /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache_size = 2;
    }

    if (n >= cache_size) {
        /* Save old size before realloc */
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_size = new_size;

        /* Compute values from old_size up to n */
        for (idx_t i = old_size; i <= n; ++i) {
            cache[i] = (seq_t)4 * cache[i - 1] + (seq_t)45 * cache[i - 2];
        }
    }

    return cache[n];
}
