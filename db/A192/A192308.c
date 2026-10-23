/*
 * A192308  1-sequence of reduction of (3n) by x^2 -> x+1.
 * https://oeis.org/A192308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*A122491(n+1) Recurrence (for n ≥ 6): a(n) = 3*a(n-1) -
 *            a(n-2) - 3*a(n-3) + a(n-4) + a(n-5) Initial values: a(1)=0,
 *            a(2)=6, a(3)=15, a(4)=39, a(5)=84 The implementation memoizes
 *            computed terms and supports arbitrary calling order
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of terms; index 0 is unused */
static idx_t cache_len = 0;   /* highest index computed (>=5 after init) */

/* Ensure values up to and including 'up_to' are computed. */
static void compute_up_to(idx_t up_to)
{
    /* Minimum capacity must contain the first five terms. */
    size_t needed_size = (up_to >= 5 ? (size_t)up_to + 1 : 6);

    if (cache == NULL) {
        cache = (seq_t *)malloc(needed_size * sizeof(seq_t));
        if (cache == NULL) return;      /* allocation failed – caller will fail */

        /* Set initial terms – they are valid for all n ≥ 1 */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)39;
        cache[5] = (seq_t)84;

        cache_len = 5;                  /* we have terms up to 5 */

        /* Compute further terms if requested up to n ≥ 6 */
        if (up_to >= 6) {
            for (idx_t i = 6; i <= up_to; ++i) {
                cache[i] = (seq_t)(3 * cache[i - 1] - cache[i - 2]
                                   - 3 * cache[i - 3] + cache[i - 4]
                                   + cache[i - 5]);
            }
            cache_len = up_to;
        }
    }
    else if (up_to > cache_len) {
        /* Grow the array to hold indices up to 'up_to' */
        size_t new_size = (size_t)up_to + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return;                /* realloc failed – abort */
        cache = tmp;

        /* Compute new terms starting just after the old highest index */
        for (idx_t i = cache_len + 1; i <= up_to; ++i) {
            cache[i] = (seq_t)(3 * cache[i - 1] - cache[i - 2]
                               - 3 * cache[i - 3] + cache[i - 4]
                               + cache[i - 5]);
        }
        cache_len = up_to;
    }
    /* If up_to ≤ cache_len nothing to do */
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 – return 0 */

    if (cache == NULL || n > cache_len) {
        compute_up_to(n);
    }
    return cache[n];
}
