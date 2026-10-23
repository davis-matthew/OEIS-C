/*
 * A011659  A binary m-sequence: expansion of reciprocal of x^4+x+1.
 * https://oeis.org/A011659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = abs(a(n‑1) – a(n‑4)), with a(0)=0, a(1)=0, a(2)=0, a(3)=1
 *            * The sequence is binary and has period 15:
 *            0,0,0,1,1,1,1,0,1,0,1,1,0,0,1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* ensure cache can hold at least n+1 elements */
    if (cache_len < 4) {
        /* initialise with first four values */
        cache = (seq_t *)malloc(sizeof(seq_t) * 4);
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache_len = 4;
    }

    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;
        /* compute new entries */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i < 4) {
                /* already initialised, but keep this for completeness */
                cache[i] = (seq_t)0;
                if (i == 3) cache[i] = (seq_t)1;
            } else {
                seq_t v1 = cache[i - 1];
                seq_t v4 = cache[i - 4];
                cache[i] = (v1 >= v4) ? (v1 - v4) : (v4 - v1);
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
