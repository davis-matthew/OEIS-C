/*
 * A041619  Denominators of continued fraction convergents to sqrt(328).
 * https://oeis.org/A041619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 1, a(1) = 9, a(2) = 325, a(3) = 2934, a(n) = 326*a(n-2) -
 *            a(n-4) (n >= 4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)325;
    if (n == 3) return (seq_t)2934;

    static seq_t *cache = NULL;
    static idx_t size   = 0;   /* number of terms already computed */

    if (n < size)           /* already computed */
        return cache[n];

    /* Initialise cache with the first four terms */
    if (size == 0) {
        cache = (seq_t *)realloc(NULL, 4 * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)325;
        cache[3] = (seq_t)2934;
        size    = 4;
    }

    if (n >= size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        for (idx_t i = size; i <= n; ++i) {
            seq_t v = cache[i-2] * (seq_t)326;
            v -= cache[i-4];
            cache[i] = v;
        }
        size = new_size;
    }

    return cache[n];
}
