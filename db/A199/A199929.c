/*
 * A199929  Trisection 2 of A199802.
 * https://oeis.org/A199929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 26, i64 n <= 53, i128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=2, a(1)=-4, a(2)=-5, a(3)=27;
 * a(n) = -a(n-1) -5*a(n-2) + a(n-3) - a(n-4)  for n>3
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base terms for n < 4 */
    if (n < 4) {
        static const seq_t init[4] = { 2, -4, -5, 27 };
        return init[n];
    }

    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* current allocation size (elements) */
    static size_t len = 3;          /* highest index that has been computed */

    size_t needed = (size_t)n + 1;
    if (cap < needed) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failure */
        cache = tmp;
        cap = new_cap;
        if (cap >= 4) {           /* ensure base terms are stored */
            cache[0] = 2;
            cache[1] = -4;
            cache[2] = -5;
            cache[3] = 27;
        }
    }

    if (n > len) {                 /* compute new terms up to n */
        for (size_t i = len + 1; i <= (size_t)n; ++i) {
            cache[i] = -cache[i-1] - (seq_t)5 * cache[i-2] + cache[i-3] - cache[i-4];
        }
        len = (size_t)n;
    }

    return cache[(size_t)n];
}
