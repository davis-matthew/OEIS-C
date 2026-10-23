/*
 * A120195  a(1)=2; a(n) = floor((17 + Sum_{j=1..n-1} a(j))/8).
 * https://oeis.org/A120195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; a(n)=floor((17 + Sum_{j=1}^{n-1} a(j))/8)
 * Range:     u8 n <= 26, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache that remembers previous terms */
    static seq_t *cache = NULL;    /* array of a(1)..a(cache_len) */
    static size_t cache_len = 0;   /* number of terms currently stored */
    static seq_t cache_sum = 0;    /* sum of all cached terms */

    if (n < 1) return 0;           /* undefined for n=0, return 0 as safe default */

    /* expand cache if necessary */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;        /* out of memory: return 0 as fallback */
        cache = tmp;
        /* compute new terms */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t val;
            if (i == 0) {                      /* a(1) */
                val = (seq_t)2;
            } else {                          /* a(n) = floor((17 + sum)/8) */
                val = (cache_sum + (seq_t)17) / (seq_t)8;
            }
            cache[i] = val;
            cache_sum += val;
        }
        cache_len = new_len;
    }
    return cache[n-1];
}
