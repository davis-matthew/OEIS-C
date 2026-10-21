/*
 * A053309  Partial sums of A053308.
 * https://oeis.org/A053309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 28, u64 n <= 74, u128 n <= 167,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A053309: Partial sums of A053308
   Recurrence: a(0)=1, a(1)=10,
                a(n)=a(n-1)+a(n-2)+C(n+8,8)  for n>=2
   (where C(x,y) is the binomial coefficient). */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Static cache for memoization (1-based length). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if ((idx_t)(n + 1) > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;                /* a(0) = 1 */
            } else if (i == 1) {
                cache[i] = (seq_t)10;               /* a(1) = 10 */
            } else {
                seq_t c = binom(i + (idx_t)8, (idx_t)8);
                cache[i] = cache[i - 1] + cache[i - 2] + c;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
