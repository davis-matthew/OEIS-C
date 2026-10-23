/*
 * A041054  Numerators of continued fraction convergents to sqrt(33).
 * https://oeis.org/A041054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 46*a(n-4)-a(n-8); a(0)=5, a(1)=6, a(2)=17, a(3)=23,
 *            a(4)=247, a(5)=270, a(6)=787, a(7)=1057; offset 0
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array holding computed terms      */
    static size_t cached_len = 0;   /* number of elements stored         */

    /* initial 8 values from the OEIS entry */
    static const seq_t init[8] = {
        (seq_t)5,   (seq_t)6,   (seq_t)17,  (seq_t)23,
        (seq_t)247, (seq_t)270, (seq_t)787, (seq_t)1057
    };

    /* grow the cache if needed */
    if ((size_t)n + 1 > cached_len) {
        size_t new_len = (size_t)n + 1;
#ifdef __cplusplus
        cache = reinterpret_cast<seq_t*>(realloc(cache, new_len * sizeof(seq_t)));
#else
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
#endif
        if (!cache) abort();

        for (idx_t i = cached_len; i <= n; ++i) {
            if (i < 8) {
                cache[i] = init[i];
            } else {
                /* use mul_small to avoid overflow for 64‑bit builds
                   and to work with arbitrary‑precision types */
                seq_t prod = mul_small(cache[i - 4], 46);
                cache[i] = prod - cache[i - 8];
            }
        }
        cached_len = new_len;
    }

    return cache[n];
}
