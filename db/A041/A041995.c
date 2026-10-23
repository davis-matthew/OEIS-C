/*
 * A041995  Denominators of continued fraction convergents to sqrt(520).
 * https://oeis.org/A041995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 12998*a(n-6) - a(n-12) (n > 11) Initial values (0‑based
 *            index): a(0)=1, a(1)=1, a(2)=5, a(3)=56, a(4)=229, a(5)=285,
 *            a(6)=12769, a(7)=13054, a(8)=64985, a(9)=727889, a(10)=2976541,
 *            a(11)=3704430 This recurrence gives the denominators of the
 *            continued‑fraction convergents to sqrt(520)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache       = NULL;  /* cached sequence values             */
    static size_t cache_sz    = 0;      /* number of entries allocated        */

    /* Base terms for indices 0 … 11 */
    static const seq_t init[12] = {
        1, 1, 5, 56, 229, 285, 12769, 13054, 64985, 727889, 2976541, 3704430
    };

    /* Extend the cache if needed */
    if ((idx_t)n >= (idx_t)cache_sz) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure – return 0 */

        cache = tmp;

        /* Compute new values from the current size up to n */
        for (size_t i = cache_sz; i <= new_sz - 1; ++i) {
            if (i < 12) {
                cache[i] = init[(size_t)i];
            } else {
                seq_t multiplier = 12998;
                cache[i] = multiplier * cache[i - 6] - cache[i - 12];
            }
        }
        cache_sz = new_sz;
    }

    return cache[(size_t)n];
}
