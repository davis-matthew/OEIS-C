/*
 * A164398  Number of binary strings of length n with no substrings equal to
 *          0001 or 1000.
 * https://oeis.org/A164398
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)-2 with a(4)=14, a(5)=25, a(6)=45
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* undefined for n < 4; caller can ignore these values. */
    if (n < 4) return 0;

    /* cache of already computed values  a(4), a(5), …  stored at cache[0], …  */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently stored */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* number of terms needed for indices 4 … n inclusive */
    idx_t needed = n - 3;

    /* grow the cache if required */
    if (needed > cache_cap) {
        idx_t new_cap = needed > cache_cap * 2 ? needed : (cache_cap == 0 ? 4 : cache_cap * 2);
        seq_t *new_arr = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!new_arr) return 0;                /* allocation failed – return 0 */
        cache      = new_arr;
        cache_cap  = new_cap;
    }

    /* compute missing terms, using the recurrence for n >= 7 */
    for (idx_t idx = cache_len; idx < needed; ++idx) {
        idx_t cur_n = idx + 4;   /* map cache index to sequence index */

        if (cur_n == 4) {
            cache[idx] = (seq_t)14;
        } else if (cur_n == 5) {
            cache[idx] = (seq_t)25;
        } else if (cur_n == 6) {
            cache[idx] = (seq_t)45;
        } else {
            /* a(n) = a(n-1) + a(n-2) + a(n-3) - 2  */
            seq_t v1 = cache[idx - 1];
            seq_t v2 = cache[idx - 2];
            seq_t v3 = cache[idx - 3];
            cache[idx] = v1 + v2 + v3 - (seq_t)2;
        }
    }
    cache_len = needed;

    return cache[n - 4];
}
