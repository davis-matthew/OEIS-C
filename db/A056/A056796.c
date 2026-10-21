/*
 * A056796  Minimal number of steps to get from 0 to n when there are 3 kinds
 *          of step: add 1, multiply by 2, multiply by 3.
 * https://oeis.org/A056796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min( a(n-1), a(n/2) if n even, a(n/3) if n≡0 mod 3 ) + 1
 *            with a(0) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached_len = 0;      /* number of elements stored: cache[0..cached_len-1] */

    /* ensure we have room for n+1 elements */
    if (cached_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* avoid undefined behaviour on allocation failure */
            abort();
        }
        cache = tmp;

        if (cached_len == 0) {
            cache[0] = (seq_t)0;
            cached_len = 1;
        }

        /* compute values for indices starting from cached_len up to n inclusive */
        const seq_t one = (seq_t)1;
        for (idx_t i = cached_len; i <= n; ++i) {
            seq_t best = cache[i - 1] + one;       /* step by +1 from i-1 */

            if (i % 2 == 0) {
                seq_t v = cache[i / 2] + one;     /* step by *2 from i/2 */
                if (v < best) best = v;
            }
            if (i % 3 == 0) {
                seq_t v = cache[i / 3] + one;     /* step by *3 from i/3 */
                if (v < best) best = v;
            }
            cache[i] = best;
        }
        cached_len = n + 1;   /* now cache[0..n] are filled */
    }

    return cache[n];
}
