/*
 * A041836  Numerators of continued fraction convergents to sqrt(439).
 * https://oeis.org/A041836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 880*a(n-4)-a(n-8) Initial terms: a(0)=20, a(1)=21,
 *            a(2)=419, a(3)=440, a(4)=18019, a(5)=18459, a(6)=368740,
 *            a(7)=387199
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t INIT[8] = {
    (seq_t)20, (seq_t)21, (seq_t)419, (seq_t)440,
    (seq_t)18019, (seq_t)18459, (seq_t)368740, (seq_t)387199
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stores computed terms */
    static size_t cache_size = 0; /* number of valid entries */

    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        /* realloc works in C and C++ when casted explicitly */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);

        /* compute missing terms, using the recurrence */
        for (size_t k = cache_size; k < new_size; ++k) {
            if (k < 8) {
                tmp[k] = INIT[k];
            } else {
                tmp[k] = tmp[k - 4] * (seq_t)880 - tmp[k - 8];
            }
        }
        cache = tmp;
        cache_size = new_size;
    }
    return cache[(size_t)n];
}
