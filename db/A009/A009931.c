/*
 * A009931  Coordination sequence for MgCu2, Mg position.
 * https://oeis.org/A009931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -3*a(n-1) -4*a(n-2) -a(n-3) +6*a(n-4) +11*a(n-5)
 *            +9*a(n-6) -9*a(n-8) -11*a(n-9) -6*a(n-10) +a(n-11) +4*a(n-12)
 *            +3*a(n-13) +a(n-14) initial values: a(0)=1, a(1)=16, a(2)=52,
 *            a(3)=130, a(4)=244, a(5)=380, a(6)=538, a(7)=784, a(8)=986,
 *            a(9)=1268, a(10)=1588, a(11)=1930, a(12)=2236, a(13)=2764...
 * Range:     u8 n <= 4, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* initial values for n = 0..14 */
    static const seq_t init[15] = {
        1, 16, 52, 130, 244, 380, 538, 784, 986, 1268,
        1588, 1930, 2236, 2764, 3086
    };

    /* static cache storing computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements currently stored */

    if (n <= 14) {
        return init[(size_t)n];
    }

    /* Ensure enough space in the cache */
    if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */
        cache = tmp;

        /* Fill missing entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i <= 14) {            /* copy initial terms if still missing */
                cache[(size_t)i] = init[(size_t)i];
            } else {                  /* compute via recurrence */
                /* Positive contributions */
                seq_t pos =
                      6 * cache[(size_t)(i-4)]
                    + 11 * cache[(size_t)(i-5)]
                    + 9 * cache[(size_t)(i-6)]
                    + cache[(size_t)(i-11)]
                    + 4 * cache[(size_t)(i-12)]
                    + 3 * cache[(size_t)(i-13)]
                    + cache[(size_t)(i-14)];

                /* Negative contributions */
                seq_t neg =
                      3 * cache[(size_t)(i-1)]
                    + 4 * cache[(size_t)(i-2)]
                    + cache[(size_t)(i-3)]
                    + 9 * cache[(size_t)(i-8)]
                    + 11 * cache[(size_t)(i-9)]
                    + 6 * cache[(size_t)(i-10)];

                cache[(size_t)i] = pos - neg;
            }
        }
        cache_size = new_size;
    }
    return cache[(size_t)n];
}
