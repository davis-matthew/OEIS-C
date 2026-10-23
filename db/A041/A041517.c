/*
 * A041517  Denominators of continued fraction convergents to sqrt(275).
 * https://oeis.org/A041517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 398*a(n-6) - a(n-12) for n>11
   with a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=7, a(5)=12,
   a(6)=391, a(7)=403, a(8)=794, a(9)=1991, a(10)=2785, a(11)=4776. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for already computed terms */
    static seq_t *cache = NULL;      /* pointer to array of seq_t */
    static idx_t cache_size = 0;     /* number of terms currently stored */

    /* initial 12 terms */
    static const seq_t init[12] = {
        1, 1, 2, 5, 7, 12,
        391, 403, 794, 1991, 2785, 4776
    };

    /* enlarge cache if necessary */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – return 0 */
            return 0;
        }
        cache = tmp;

        /* compute terms that are missing */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 12) {
                cache[i] = init[i];
            } else {
                cache[i] = (seq_t)398 * cache[i - 6] - cache[i - 12];
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
