/*
 * A117826  First four terms of the sequence are doubled, then those numbers
 *          are tripled and then those numbers are quadrupled, etc.
 * https://oeis.org/A117826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(4n-3) = n! ; a(4n-2) = 2*n! ; a(4n-1) = 3*n! ; a(4n) = 4*n!
 * Range:     u8 n <= 18, u16 n <= 29, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* safety for out‑of‑range index */

    /* ceil(n / 4) computed without overflow */
    idx_t k = n / 4 + (n % 4 != 0);

    /* Cache factorials: cache[i] holds i! for 0 <= i <= cached */
    static seq_t *cache = NULL;
    static idx_t cached = 0;                    /* largest factorial computed */

    if (cached < k) {
        /* Allocate or grow buffer */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      sizeof(seq_t) * (k + 1));
        if (tmp == NULL) {
            /* Allocation failed: recompute factorial on the fly */
            seq_t f = 1;
            for (idx_t i = 2; i <= k; ++i) f *= (seq_t)i;

            seq_t mult = (seq_t)((n % 4 == 0) ? 4 :
                                 (n % 4 == 3) ? 3 :
                                 (n % 4 == 2) ? 2 : 1);
            return f * mult;
        }
        cache = tmp;

        if (cached == 0) {
            /* First time: set 0! = 1 and compute all up to k */
            cache[0] = 1;
            for (idx_t i = 1; i <= k; ++i)
                cache[i] = cache[i - 1] * (seq_t)i;
        } else {
            /* Append additional factorial values */
            for (idx_t i = cached + 1; i <= k; ++i)
                cache[i] = cache[i - 1] * (seq_t)i;
        }
        cached = k;
    }

    seq_t mult = (seq_t)((n % 4 == 0) ? 4 :
                         (n % 4 == 3) ? 3 :
                         (n % 4 == 2) ? 2 : 1);

    return cache[k] * mult;
}
