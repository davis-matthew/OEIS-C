/*
 * A063320  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          47 ).
 * https://oeis.org/A063320
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A063320: Dimension of weight‑n cuspidal newforms for Γ₁(47)
    Formula: a(n) = a(n-4) + a(n-6) - a(n-10)  for n > 13
    Initial values (offset 2):
        a(2) = -1
        a(3) = 70
        a(4) = 161
        a(5) = 253
        a(6) = 345
        a(7) = 437
        a(8) = 529
        a(9) = 621
        a(10)= 713
        a(11)= 805
        a(12)= 897
        a(13)= 987
*/

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation table   */
static idx_t  cache_size = 0;    /* allocated size      */
static idx_t  filled = 1;        /* highest index computed */

static const seq_t init_vals[] = {
    0,          /* 0 */
    0,          /* 1 */
   -1,          /* 2 */
    70,         /* 3 */
   161,         /* 4 */
   253,         /* 5 */
   345,         /* 6 */
   437,         /* 7 */
   529,         /* 8 */
   621,         /* 9 */
   713,         /*10 */
   805,         /*11 */
   897,         /*12 */
   987          /*13 */
};

/* Ensure the cache is large enough and initialised */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initial allocation: at least up to 14 */
        cache_size = (n > 14 ? n + 1 : 15);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = 0;
        /* copy known values */
        for (idx_t i = 0; i < sizeof(init_vals) / sizeof(init_vals[0]); ++i)
            cache[i] = init_vals[i];
        filled = sizeof(init_vals) / sizeof(init_vals[0]) - 1; /* 13 */
    } else if (n >= cache_size) {
        /* grow cache */
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* values below the offset are irrelevant */

    ensure_cache(n);

    /* compute any missing terms up to n */
    for (idx_t i = filled + 1; i <= n; ++i) {
        if (i <= 13)
            continue;           /* already initialised */
        /* recurrence: a(i) = a(i-4) + a(i-6) - a(i-10) */
        cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
    }
    filled = n;
    return cache[n];
}
