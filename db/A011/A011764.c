/*
 * A011764  a(n) = 3^(2^n) (or: write in base 3, read in base 9).
 * https://oeis.org/A011764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(2^n) (a(0)=3, a(n+1)=a(n)^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;   /* cache[0] = a(0) */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_len = 0;  /* number of stored terms (cache_len == N means indices 0..N-1 stored) */

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) { return (seq_t)0; } /* allocation failed – return 0, caller must handle */
        cache[0] = (seq_t)3;  /* a(0) = 3 */
        cache_len = 1;
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) { return (seq_t)0; } /* allocation failed – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values using recurrence a(i) = a(i-1)^2 */
    for (idx_t i = cache_len; (idx_t)i <= n; ++i) {
        cache[i] = cache[i - 1] * cache[i - 1];
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
