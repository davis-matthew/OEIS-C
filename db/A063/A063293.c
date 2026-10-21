/*
 * A063293  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          20 ).
 * https://oeis.org/A063293
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(2) = -1, a(3)=3, a(4)=10, a(5)=17, a(6)=22, a(7)=29, *
 *            a(8)=34, a(9)=45, a(10)=46, a(11)=57, a(12)=58, a(13)=71, * and
 *            for n>13: a(n) = a(n-4) + a(n-6) - a(n-10)
 * Range:     i8 n <= 22, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* undefined for n < 2 */

    /* zero‑based cache: cache[0] ↔ a(2), cache[1] ↔ a(3), ... */
    static seq_t *cache = NULL;
    static size_t cap = 0;     /* capacity of cache array      */
    static size_t cached = 0;  /* number of already computed entries */

    idx_t pos = n - 2;  /* zero‑based index for a(n) */

    /* Ensure the cache is large enough */
    if (cap == 0 || pos + 1 > cap) {
        size_t newcap = (cap == 0) ? 16 : cap;
        while (newcap < (size_t)(pos + 1))
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            return 0;  /* allocation failure (unlikely) */
        cache = tmp;
        cap = newcap;

        /* Initialise the first 12 terms if they have not been set yet */
        if (cached < 12) {
            static const seq_t init[12] = {
                -1, 3, 10, 17, 22, 29, 34, 45, 46, 57, 58, 71
            };
            for (size_t i = 0; i < 12; ++i)
                cache[i] = init[i];
            cached = 12;
        }
    }

    /* Compute missing terms up to the required position */
    for (size_t j = cached; j <= (size_t)pos; ++j) {
        if (j >= 12) {  /* recurrence starts at a(14) (index 12) */
            cache[j] = cache[j - 4] + cache[j - 6] - cache[j - 10];
        }
    }
    cached = (size_t)pos + 1;

    return cache[(size_t)pos];
}
