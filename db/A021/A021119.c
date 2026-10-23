/*
 * A021119  Decimal expansion of 1/115.
 * https://oeis.org/A021119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = a(n-1) - a(n-11) + a(n-12) for n > 12 Initial terms:
 *            a(0)=0, a(1)=0, a(2)=8, a(3)=6, a(4)=9, a(5)=5, a(6)=6, a(7)=5,
 *            a(8)=2, a(9)=1, a(10)=7, a(11)=3, a(12)=9 (Decimal expansion of
 *            1/115)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */

    /* Convert index to a size_t that can be used for array access. */
    size_t idx = (size_t)n;

    if (idx >= cache_len) {
        size_t new_len = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) { /* allocation failure – return 0 as a safe default */
            return (seq_t)0;
        }
        cache = tmp;

        /* Store the known initial terms up to index 12. */
        static const seq_t INIT[13] = {
            0, 0, 8, 6, 9, 5, 6, 5, 2, 1, 7, 3, 9
        };
        size_t i;
        for (i = cache_len; i < sizeof(INIT)/sizeof(INIT[0]) && i < new_len; ++i)
            cache[i] = INIT[i];

        /* Compute further terms via the recurrence. */
        if (new_len > sizeof(INIT)/sizeof(INIT[0])) {
            size_t start = (cache_len > sizeof(INIT)/sizeof(INIT[0])) ?
                            cache_len : sizeof(INIT)/sizeof(INIT[0]);
            for (i = start; i < new_len; ++i) {
                int val = (int)cache[i-1] - (int)cache[i-11] + (int)cache[i-12];
                cache[i] = (seq_t)val;
            }
        }

        cache_len = new_len;
    }
    return cache[idx];
}
