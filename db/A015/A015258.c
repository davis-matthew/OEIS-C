/*
 * A015258  Gaussian binomial coefficient [ n,2 ] for q = -7.
 * https://oeis.org/A015258
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 43*a(n-1) + 301*a(n-2) - 343*a(n-3), n >= 5 Initial
 *            values: a(2) = 1, a(3) = 43, a(4) = 2150. Offset 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;
static size_t cache_len = 0;

/* Compute Gaussian binomial coefficient [n,2] for q = -7 */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                           /* Undefined for n < 2, unused */

    /* Ensure cache can hold indices up to n */
    if (cache_len <= n) {
        size_t new_len = n + 1;              /* indices 0..n inclusive */
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
        if (!new_cache)
            exit(EXIT_FAILURE);             /* allocation failure */
        seq_cache = new_cache;

        /* Initialize new entries if cache was empty */
        for (size_t i = cache_len; i <= n; ++i) {
            if (i == 2)
                seq_cache[i] = (seq_t)1;
            else if (i == 3)
                seq_cache[i] = (seq_t)43;
            else if (i == 4)
                seq_cache[i] = (seq_t)2150;
            else if (i >= 5)
                seq_cache[i] = (seq_t)43 * seq_cache[i - 1]
                               + (seq_t)301 * seq_cache[i - 2]
                               - (seq_t)343 * seq_cache[i - 3];
        }
        cache_len = new_len;
    }

    return seq_cache[n];
}
