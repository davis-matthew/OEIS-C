/*
 * A015020  q-factorial numbers for q=-7.
 * https://oeis.org/A015020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ∏_{k=1..n} ((-7)^k - 1) / (-8) Recurrence: a(0) = 1, a(n)
 *            = ((-7)^n - 1) * a(n‑1) / (-8)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 7, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* q‑factorial numbers for q = -7 */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* array of a(0) … a(m) */
    static idx_t  cache_size = 0;      /* number of elements stored so far */
    static seq_t   power     = (seq_t)-7; /* (-7)^1, used for the next k */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* allocate space for a(0) */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL)   /* allocation failure – unlikely but safe */
            return (seq_t)0;
        cache[0]  = (seq_t)1;   /* a(0) = 1 */
        cache_size = 1;         /* we have a(0) stored */
        return n == 0 ? cache[0] : 0; /* for n == 0 just return the value */
    }

    /* if n is already cached, return it immediately */
    if (n < cache_size)
        return cache[n];

    /* extend cache up to n (both inclusive) */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (tmp == NULL)   /* allocation failure – unlikely but safe */
        return (seq_t)0;
    cache = tmp;

    /* compute missing values */
    for (idx_t k = cache_size; k <= n; ++k) {
        /* factor = (-7)^k - 1   (power currently holds (-7)^k) */
        seq_t factor = power - (seq_t)1;

        /* a(k) = a(k-1) * factor / (-8)   (division is exact) */
        seq_t mult = cache[k - 1] * factor;
        seq_t val  = mult / (seq_t)(-8);

        cache[k] = val;

        /* prepare power for the next k: (-7)^(k+1) = power * (-7) */
        power *= (seq_t)-7;
    }

    cache_size = n + 1;
    return cache[n];
}
