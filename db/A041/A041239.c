/*
 * A041239  Denominators of continued fraction convergents to sqrt(131).
 * https://oeis.org/A041239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=9, a(3)=101, a(4)=413, a(5)=927,
   a(6)=20807, a(7)=42541, a(8)=190971, a(9)=2143222, a(10)=8763859,
   a(11)=19670940,
   a(n) = 21220*a(n-6) - a(n-12)   for n >= 12 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0 … 11 */
    static const seq_t init[12] = {
        (seq_t)1, (seq_t)2, (seq_t)9,
        (seq_t)101,
        (seq_t)413,
        (seq_t)927,
        (seq_t)20807,
        (seq_t)42541,
        (seq_t)190971,
        (seq_t)2143222,
        (seq_t)8763859,
        (seq_t)19670940
    };

    if (n < 12)
        return init[n];

    /* Cache dynamic storage of values up to the largest n requested. */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;          /* number of cached elements */

    if (n >= cap) {                  /* need to grow cache */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* memory allocation failure */
        cache = tmp;

        /* initialise any missing indices from 0 .. new_cap-1 */
        for (idx_t i = cap; i < new_cap; ++i) {
            if (i < 12) {
                cache[i] = init[i];
            } else {
                cache[i] = (seq_t)21220 * cache[i - 6] - cache[i - 12];
            }
        }
        cap = new_cap;
    }

    return cache[n];
}
