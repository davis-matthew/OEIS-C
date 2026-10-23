/*
 * A163460  a(n) = 16*a(n-1) - 62*a(n-2) for n > 1; a(0) = 1, a(1) = 9.
 * https://oeis.org/A163460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 62*a(n-2), a(0) = 1, a(1) = 9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of already computed values */
static idx_t cache_size = 0;  /* number of entries stored in cache */

/* Returns the nth term of OEIS A163460, using a memoised recurrence. */
seq_t a(idx_t n)
{
    /* Ensure cache can hold values up to n. */
    if (n >= cache_size) {
        /* Grow cache. */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failed – return 0 (undefined behaviour) */
            return (seq_t)0;
        }
        cache = tmp;
        /* Compute new entries. */
        if (cache_size == 0) {             /* first call */
            cache[0] = (seq_t)1;
            cache_size = 1;
            if (n >= 1) {
                cache[1] = (seq_t)9;
                cache_size = 2;
            }
        }
        /* Fill entries beyond the current cache_size up to n. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)9;
            } else {
                /* The recurrence uses only integer arithmetic. */
                cache[i] = (seq_t)16 * cache[i - 1] - (seq_t)62 * cache[i - 2];
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
