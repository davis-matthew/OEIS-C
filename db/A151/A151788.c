/*
 * A151788  Partial sums of A151787.
 * https://oeis.org/A151788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 For n even (n ≥ 2): a(n) = 3 * a(n/2) + 1 For n odd (n
 *            > 1): a(n) = 2 * a((n-1)/2) + a((n+1)/2) + 1
 * Range:     u8 n <= 27, u16 n <= 933, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: index 0 is unused; 0 value indicates uncomputed */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* grow cache to at least n */
    if (n > cap) {
        size_t new_cap = (n > 16 ? n : 16);
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failure */
        for (size_t i = cap + 1; i <= new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }

    if (cache[n] != 0) return cache[n]; /* memoised result */

    seq_t result;
    if (n == 1) {
        result = (seq_t)1;
    } else if ((n & 1) == 0) {            /* even n */
        idx_t half = n >> 1;
        result = (seq_t)3 * a(half) + (seq_t)1;
    } else {                              /* odd n ≥ 3 */
        idx_t k1 = (n - 1) >> 1;           /* (n-1)/2 */
        idx_t k2 = (n + 1) >> 1;           /* (n+1)/2 */
        result = (seq_t)2 * a(k1) + a(k2) + (seq_t)1;
    }

    cache[n] = result;                    /* store for future calls */
    return result;
}
