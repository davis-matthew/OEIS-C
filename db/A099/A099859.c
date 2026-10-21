/*
 * A099859  A Chebyshev transform of A006053 related to the knot 7_1.
 * https://oeis.org/A099859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=0, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=0 Recurrence for
 *            n≥6: a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) - a(n-6)
 * Range:     i8 n <= 81, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t size = 0;

    /* initial terms */
    static const seq_t init[6] = { 0, 1, 1, 1, 1, 0 };

    /* expand cache if necessary */
    if (n >= size) {
        /* new size: at least n+1 */
        idx_t new_size = (n + 1 > size * 2) ? (n + 1) : (size * 2 + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* out of memory fallback */
        cache = tmp;
        /* initialise new part */
        for (idx_t i = size; i < new_size; ++i) {
            if (i < 6) {
                cache[i] = init[i];
            } else {
                /* compute via recurrence using already computed values */
                cache[i] = cache[i-1] - cache[i-2] + cache[i-3]
                         - cache[i-4] + cache[i-5] - cache[i-6];
            }
        }
        size = new_size;
    }
    return cache[n];
}
