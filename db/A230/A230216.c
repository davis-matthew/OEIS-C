/*
 * A230216  Number of binary strings of length n avoiding "squares" (that is,
 *          repeated blocks of the form xx) with |x| = 3.
 * https://oeis.org/A230216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) for n > 5 Initial values:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=32
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* current allocated capacity */
static idx_t cache_len = 0;   /* number of elements that have been computed */

/* Base values for indices 0 … 5 */
static const seq_t base_vals[6] = { (seq_t)1, (seq_t)2, (seq_t)4,
                                   (seq_t)8, (seq_t)16, (seq_t)32 };

seq_t a(idx_t n)
{
    /* Ensure the array is large enough to hold indices up to n */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {               /* allocation failure – return 0 */
            return (seq_t)0;
        }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    if (n >= cache_len) {
        /* First fill in the fixed base values up to index 5 */
        for (idx_t i = cache_len; i <= n && i <= 5; ++i)
            cache[i] = base_vals[i];

        /* Then use the recurrence for indices > 5 */
        for (idx_t i = cache_len; i <= n; ++i)
            if (i > 5)
                cache[i] = cache[i-1] + cache[i-2] + cache[i-3];

        cache_len = n + 1;
    }

    return cache[n];
}
