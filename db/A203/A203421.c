/*
 * A203421  Reciprocal of Vandermonde determinant of (1,1/2,...,1/n).
 * https://oeis.org/A203421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Product_{i=2..n} (-i)^(i-1) (with a(0)=a(1)=1)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 7, i128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

static seq_t *cache = NULL;       /* memoized values for 0 .. cache_size-1 */
static idx_t  cache_size = 0;     /* next index to be computed */

/* external linkage as required */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)          /* Base cases */
        return (seq_t)1;

    /* Initialise cache for indices 0 and 1 */
    if (cache_size == 0) {
        cache = (seq_t*)realloc(cache, 2 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* Extend cache up to n */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – fall back to computing without cache */
            cache = NULL;
            cache_size = 0;
        } else {
            cache = tmp;
            for (idx_t i = cache_size; i <= n; ++i) {
                seq_t base = -(seq_t)i;          /* (-i) */
                seq_t exp  = (seq_t)(i - 1);      /* (i-1) */
                seq_t power = big_pow(base, exp);
                cache[i] = cache[i - 1] * power;
            }
            cache_size = new_size;
        }
    }

    /* Return memoized or newly computed value */
    if (cache && n < cache_size)
        return cache[n];

    /* If memoisation failed, compute the value directly */
    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t power = big_pow(-(seq_t)i, (seq_t)(i - 1));
        result *= power;
    }
    return result;
}
