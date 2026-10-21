/*
 * A082764  Trinomial transform of the Pell numbers (A000129).
 * https://oeis.org/A082764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) - 7*a(n-2), a(0)=1, a(1)=8 (Trinomial transform
 *            of the Pell numbers)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    /* Initialise cache for n>=2 if needed. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) exit(1);          /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache_len = 2;
    }

    /* Expand cache to hold term n. */
    if ((idx_t)cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL) exit(1);      /* out of memory */
        cache = new_cache;
        /* Compute missing terms using recurrence. */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)10 * cache[i - 1] - (seq_t)7 * cache[i - 2];
        }
        cache_len = new_len;
    }
    return cache[n];
}
