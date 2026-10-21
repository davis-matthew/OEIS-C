/*
 * A099446  A Chebyshev transform of A057083.
 * https://oeis.org/A099446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) satisfies the 4‑term linear recurrence * a(0)=1, a(1)=3,
 *            a(2)=5, a(3)=3 * a(n) - 3 a(n-1) + 5 a(n-2) - 3 a(n-3) + a(n-4)
 *            = 0 for n ≥ 4
 * Range:     i8 n <= 8, i16 n <= 18, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;          /* current allocated size */

    /* determine how many terms are required */
    size_t need = (size_t)n + 1;
    if (cap < need) {
        size_t new_cap = need;
        if (new_cap < 4) new_cap = 4;   /* at least space for the first four terms */

        /* reallocate the cache, handle C++ void* cast */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            exit(EXIT_FAILURE); /* allocation failed */
        }
        cache = tmp;

        /* compute new terms, starting from the previously filled index */
        for (size_t i = cap; i < new_cap; ++i) {
            if (i == 0)      cache[0] = 1;
            else if (i == 1) cache[1] = 3;
            else if (i == 2) cache[2] = 5;
            else if (i == 3) cache[3] = 3;
            else
                cache[i] = (seq_t)3 * cache[i-1]
                           - (seq_t)5 * cache[i-2]
                           + (seq_t)3 * cache[i-3]
                           -  cache[i-4];
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
