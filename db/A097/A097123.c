/*
 * A097123  Expansion of (1-x)^2/((1-x)^3 - 4*x^3).
 * https://oeis.org/A097123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 5*a(n-3), a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached values */
    static size_t cache_len = 0;    /* number of entries currently valid */
    static size_t cache_cap = 0;    /* allocated capacity          */

    /* enlarge cache if necessary */
    size_t needed = (size_t)(n + 1);
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { abort(); }            /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms */
    for (size_t i = cache_len; i < needed; ++i) {
        seq_t val;
        if (i <= 2) {
            val = (seq_t)1;
        } else {
            seq_t t1 = cache[i-1] * (seq_t)3;
            seq_t t2 = cache[i-3] * (seq_t)5;
            val = t1 + t2;
            val = val - cache[i-2] * (seq_t)3;
        }
        cache[i] = val;
    }
    cache_len = needed;

    return cache[(size_t)n];
}
