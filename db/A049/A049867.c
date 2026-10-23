/*
 * A049867  a(n) = Sum{a(k): k=0,1,2,...,n-4,n-2,n-1}; a(n-3) is not a
 *          summand; initial terms are 1,1,4.
 * https://oeis.org/A049867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = Sum_{k=0}^n-4, n-2, n-1} a(k) (a(n-3) omitted) *
 *            Recurrence (n ≥ 4): * a(n) = 2*a(n-1) - a(n-3) + a(n-4) *
 *            Initial values: * a(0)=1, a(1)=1, a(2)=4, a(3)=5
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds computed values up to the largest index seen */
    static seq_t *cache = NULL;
    static size_t cached_size = 0;

    /* extend cache if necessary */
    if ((size_t)n >= cached_size) {
        size_t new_size = (size_t)n + 1;
        /* realloc returns void*, cast for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – keep old cache and return zero */
            return 0;
        }
        cache = tmp;

        /* compute terms from the current cached_size up to n */
        for (idx_t i = (idx_t)cached_size; i < (idx_t)new_size; ++i) {
            seq_t val;
            if (i == 0)          val = (seq_t)1;
            else if (i == 1)     val = (seq_t)1;
            else if (i == 2)     val = (seq_t)4;
            else if (i == 3)     val = (seq_t)5;
            else
                val = (seq_t)(2 * cache[(size_t)(i - 1)] -
                               cache[(size_t)(i - 3)] +
                               cache[(size_t)(i - 4)]);
            cache[(size_t)i] = val;
        }
        cached_size = new_size;
    }

    return cache[(size_t)n];
}
