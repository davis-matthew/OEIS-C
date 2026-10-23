/*
 * A015105  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=-8.
 * https://oeis.org/A015105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 7, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 1
    a(n+1) = Σ_{i=0..n} (-8)^i * a(i) * a(n-i)   (q = -8) */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;       /* memoized values of a(n)           */
static size_t cache_len = 0;      /* number of computed entries          */
static const seq_t Q = (seq_t)-8; /* constant q = -8                    */

seq_t a(idx_t n)
{
    /* Expand cache to contain values up to index n, if needed.       */
    if ((size_t)n >= cache_len)
    {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                       /* allocation failure  */
        cache = tmp;
        for (size_t k = cache_len; k < new_len; ++k)
        {
            if (k == 0)                          /* a(0) = 1            */
            {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            seq_t factor = 1;                     /* (-8)^0              */
            for (size_t i = 0; i < k; ++i)
            {
                seq_t term = cache[i] * cache[k - 1 - i];
                term = term * factor;
                sum += term;
                factor *= Q;                      /* next power of -8   */
            }
            cache[k] = sum;
        }
        cache_len = new_len;
    }
    return cache[(size_t)n];
}
