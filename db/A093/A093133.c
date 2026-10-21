/*
 * A093133  Third binomial transform of Fib(3n-3) divided by 2.
 * https://oeis.org/A093133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) - 20*a(n-2) with a(0) = 1, a(1) = 3, a(2) = 10
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[n] holds a(n) */
static idx_t cache_len = 0;      /* number of values stored in cache */

/* compute a(n) and cache values up to n */
seq_t a(idx_t n)
{
    /* handle small n directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)10;

    /* allocate cache if first time */
    if (cache == NULL) {
        cache_len = 3;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)10;
    }

    /* extend cache to required size */
    if (n >= cache_len) {
        /* reallocate to hold up to n */
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
        if (new_cache == NULL) {
            /* allocation failed – abort (should not happen in normal runs) */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;
        /* compute missing values */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* use recurrence: a_i = 10*a_{i-1} - 20*a_{i-2} */
            cache[i] = ((seq_t)10) * cache[i - 1] - ((seq_t)20) * cache[i - 2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
