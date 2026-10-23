/*
 * A041571  Denominators of continued fraction convergents to sqrt(303).
 * https://oeis.org/A041571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 5048*a(n-6) - a(n-12) for n > 11, with initial values
 *            a(0)=1, a(1)=2, a(2)=5, a(3)=27, a(4)=59, a(5)=145, a(6)=4989,
 *            a(7)=10123, a(8)=25235, a(9)=136298, a(10)=297831, a(11)=731960
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* memoised values */
    static size_t cap = 0;           /* allocated capacity */

    size_t sn = (size_t)n;           /* n fits into size_t for n <= 10 000 */

    /* initial allocation and values */
    if (!cache) {
        cap = sn + 1;
        if (cap < 12) cap = 12;      /* must store the 12 seeds */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failed */

        static const seq_t init[12] = {
            (seq_t)1,   (seq_t)2,   (seq_t)5,    (seq_t)27,
            (seq_t)59,  (seq_t)145, (seq_t)4989, (seq_t)10123,
            (seq_t)25235,(seq_t)136298,(seq_t)297831,(seq_t)731960
        };
        for (size_t i = 0; i < 12 && i < cap; ++i)
            cache[i] = init[i];

        /* compute additional terms up to current capacity */
        for (size_t i = 12; i < cap; ++i)
            cache[i] = (seq_t)5048 * cache[i - 6] - cache[i - 12];
    }

    /* extend cache if required */
    if (sn >= cap) {
        size_t old_cap = cap;
        size_t new_cap = sn + 1;     /* exact capacity needed */
        if (new_cap < old_cap) new_cap = old_cap;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cap = new_cap;

        for (size_t i = old_cap; i <= sn; ++i)
            cache[i] = (seq_t)5048 * cache[i - 6] - cache[i - 12];
    }

    return cache[sn];
}
