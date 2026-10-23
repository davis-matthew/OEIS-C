/*
 * A156091  One fourth of the alternating sum of the squares of the first n
 *          Fibonacci numbers with index divisible by 3.
 * https://oeis.org/A156091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0, a(1) = -1, a(2) = 15 Recurrence for n >= 3: a(n) =
 *            -17*a(n-1) + 17*a(n-2) + a(n-3) - 2
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 16, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Reserve space up to n */
    if (n >= cache_len) {
        size_t new_cap = (cache_cap == 0) ? 4 : cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;          /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failure */
        cache   = tmp;
        cache_cap = new_cap;

        /* Initialise new entries */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t val;
            if (i == 0)      val = 0;
            else if (i == 1) val = (seq_t)-1;
            else if (i == 2) val = 15;
            else {
                seq_t a1 = cache[i-1];
                seq_t a2 = cache[i-2];
                seq_t a3 = cache[i-3];
                val = -((seq_t)17 * a1) + ((seq_t)17 * a2) + a3 - 2;
            }
            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
