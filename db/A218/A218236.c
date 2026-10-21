/*
 * A218236  Hilltop maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements not exceeded by any king-move
 *          neighbor in a random 0..3 n X 2 array.
 * https://oeis.org/A218236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * (a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7)),
 *            with initial values a(1)=3, a(2)=15, a(3)=63, a(4)=255,
 *            a(5)=1017, a(6)=4065, a(7)=16257
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A218236.
   The function memoizes results so that any order of calls is efficient. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0) return 0;   /* out of offset range – safe default */

    /* initialise cache with the first 7 terms */
    if (cache == NULL) {
        cache_len = 7;
        cache = (seq_t*)malloc((cache_len + 1) * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)15;
        cache[3] = (seq_t)63;
        cache[4] = (seq_t)255;
        cache[5] = (seq_t)1017;
        cache[6] = (seq_t)4065;
        cache[7] = (seq_t)16257;
    }

    /* return cached value if already computed */
    if (n <= cache_len) return cache[n];

    /* grow cache to hold up to n */
    idx_t new_len = n;
    seq_t *new_cache = (seq_t*)realloc(cache, (new_len + 1) * sizeof(seq_t));
    if (new_cache == NULL) {
        /* allocation failed – unlikely in tests */
        return 0;
    }
    cache = new_cache;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= new_len; ++i) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= 7; ++j) {
            sum += cache[i - j];
        }
        cache[i] = sum * (seq_t)3;
    }

    cache_len = new_len;
    return cache[n];
}
