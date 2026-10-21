/*
 * A053739  Partial sums of A014166.
 * https://oeis.org/A053739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + C(n+4,4); a(0)=1, a(1)=6
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 36, u64 n <= 82, u128 n <= 175,
 *            big any n
 * Helpers:   binom_inline
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom_inline.h"

seq_t a(idx_t n)
{
    /* memoization cache (static, shared among calls) */
    static seq_t *cache = NULL;   /* array holding a[0 .. size-1] */
    static idx_t size  = 0;       /* current length of cache          */

    /* enlarge cache if necessary */
    if (n >= size) {
        idx_t oldSize = size;
        size = n + 1;
        cache = (seq_t *)realloc(cache, size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure – unlikely */

        /* compute the missing entries sequentially */
        for (idx_t i = oldSize; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;                       /* a(0) */
            } else if (i == 1) {
                cache[1] = cache[0] + binom_inline(5, 4);   /* a(1) */
            } else {
                cache[i] = cache[i-1] + cache[i-2]
                           + binom_inline(i + 4, 4);          /* recurrence */
            }
        }
    }
    return cache[n];
}
