/*
 * A153315  Denominators of continued fraction convergents to sqrt(5/4).
 * https://oeis.org/A153315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=8; a(2n)=2*a(2n-1)+a(2n-2); a(2n+1)=8*a(2n)+a(2n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_n = 0;

    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)8;

    if (cache == NULL) {
        grow_seq(&cache, &cap, 2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        max_n = 1;
    }

    if (n > max_n) {
        grow_seq(&cache, &cap, (size_t)n + 1);
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if ((i & (idx_t)1) == 0) {            /* even index */
                cache[i] = cache[i - 1] * (seq_t)2 + cache[i - 2];
            } else {                              /* odd index */
                cache[i] = cache[i - 1] * (seq_t)8 + cache[i - 2];
            }
        }
        max_n = n;
    }
    return cache[n];
}
