/*
 * A240328  Inverse of 37th cyclotomic polynomial.
 * https://oeis.org/A240328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the expansion of 1/(1 + x + ... +
 *            x^36) (i.e. the inverse of the 37th cyclotomic polynomial)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*  External linkage as required. */
seq_t a(idx_t n)
{
    /*  static cache of previously computed terms   */
    static seq_t *cache   = NULL;   /* dynamic array of values a(0)...a(last) */
    static size_t  cap    = 0;      /* allocated size of cache          */
    static idx_t   last   = 0;      /* highest index that has been computed */

    /*  Initialise cache on first call  */
    if (cache == NULL)
    {
        cap   = 1;                                 /* at least a[0] */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;               /* allocation failure – unlikely */
        cache[0] = (seq_t)1;                       /* a(0) = 1 */
        last    = 0;
    }

    /*  Ensure we have enough space for a(n)  */
    if ((idx_t)cap <= n)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;                 /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /*  Compute any missing terms up to n  */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        seq_t sum = 0;
        idx_t start = (i >= 36) ? (i - 36) : 0;     /* indices of a[i-1]..a[i-36] */
        for (idx_t j = start; j < i; ++j)
            sum += cache[j];
        cache[i] = -sum;                            /* recurrence: a(i) = –∑_{k=1}^{36} a(i-k) */
    }
    last = n;

    return cache[n];
}
