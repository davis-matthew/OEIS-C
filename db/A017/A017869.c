/*
 * A017869  Expansion of 1/(1-x^8-x^9-x^10-x^11).
 * https://oeis.org/A017869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8)+a(n-9)+a(n-10)+a(n-11) for n>10; a(0)=1,
 *            a(1..7)=0, a(8..11)=1
 * Range:     u8 n <= 53, u16 n <= 90, u32 n <= 165, u64 n <= 316,
 *            u128 n <= 617, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* ensure cache covers indices up to 11 at start */
    if (cache_len == 0) {
        cache_len = 12; /* indices 0..11 */
        cache      = (seq_t *)calloc((size_t)cache_len, sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out of memory fallback */

        cache[0] = (seq_t)1;             /* a(0) */
        /* a(1..7) are already zero */
        cache[8] = cache[9] = cache[10] = cache[11] = (seq_t)1;
    }

    /* grow cache if needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* out of memory fallback */
        cache      = tmp;
        /* initialize newly allocated part to zero */
        for (idx_t i = cache_len; i < new_len; ++i)
            cache[i] = (seq_t)0;
        /* compute values for newly added indices */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i <= 11) {
                /* base values already set except i=8..11 which are 1 already */
                continue;
            }
            cache[i] = cache[i-8] + cache[i-9] + cache[i-10] + cache[i-11];
        }
        cache_len = new_len;
    }

    return cache[n];
}
