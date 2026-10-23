/*
 * A131132  a(n) = a(n-1) + a(n-2) + 1 if n is a multiple of 6, otherwise a(n)
 *          = a(n-1) + a(n-2).
 * https://oeis.org/A131132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1; a(n)=a(n-1)+a(n-2)+1 if n%6==0, else
 *            a(n)=a(n-1)+a(n-2) (n>=2)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache of already computed terms: indices [0 .. cache_len-1] */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* need to compute up to n if not already done */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        if (new_len < 2) new_len = 2;   /* ensure room for a(0) and a(1) */

#if defined(__cplusplus)
        /* C++: use new[]/delete[] to ensure proper construction/destruction */
        seq_t *tmp = new seq_t[new_len];
        /* copy existing values */
        for (idx_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        /* C: malloc/realloc handles plain integer types */
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;
#endif

        /* compute any new terms */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)1;
            } else {
                if (i % 6 == 0) {
                    cache[i] = cache[i-1] + cache[i-2] + (seq_t)1;
                } else {
                    cache[i] = cache[i-1] + cache[i-2];
                }
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
