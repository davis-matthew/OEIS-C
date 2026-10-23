/*
 * A028155  Expansion of 1/((1-4x)(1-8x)(1-9x)(1-10x)).
 * https://oeis.org/A028155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10^(n+4)-24*9^(n+3)+15*8^(n+3)-4^(n+3))/120
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=31, a(2)=611, a(3)=9779
   a(n)=31*a(n-1)-350*a(n-2)+1688*a(n-3)-2880*a(n-4) for n>=4 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cached sequence values */
    static idx_t cache_len = 0;          /* number of stored terms */

    if (n >= cache_len) {
        /* Grow cache to accommodate terms up to n */
        idx_t new_len = n + 1;
        seq_t *p = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!p && new_len > 0) {
            /* allocation failed – terminate with zero value */
            return (seq_t)0;
        }
        cache = p;
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)31;
            else if (i == 2)
                cache[i] = (seq_t)611;
            else if (i == 3)
                cache[i] = (seq_t)9779;
            else {
                seq_t t1 = (seq_t)31   * cache[i-1];
                seq_t t3 = (seq_t)1688 * cache[i-3];
                seq_t res = t1 + t3;
                seq_t t2 = (seq_t)350  * cache[i-2];
                seq_t t4 = (seq_t)2880 * cache[i-4];
                res -= t2;
                res -= t4;
                cache[i] = res;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
