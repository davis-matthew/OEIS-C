/*
 * A063322  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          49 ).
 * https://oeis.org/A063322
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

/*  OEIS A063322
    Formula (recurrence for n > 13)
      a(n) = a(n-4) + a(n-6) - a(n-10)
    Initial values (offset 2)
      a(2) = -1,  a(3) = 69,  a(4) = 164,  a(5) = 258,  a(6) = 352,
      a(7) = 446,  a(8) = 540,  a(9) = 634,  a(10)= 728, a(11)= 822,
      a(12)= 916,  a(13)=1011
*/
#include <stdlib.h>

static const seq_t init_vals[14] = {
    0,   /* 0 (unused) */
    0,   /* 1 (unused) */
   -1,  /* 2 */
    69,  /* 3 */
   164,  /* 4 */
   258,  /* 5 */
   352,  /* 6 */
   446,  /* 7 */
   540,  /* 8 */
   634,  /* 9 */
   728,  /*10 */
   822,  /*11 */
   916,  /*12 */
  1011   /*13 */
};

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cached_max = 1;   /* largest n for which a(n) is stored */

seq_t a(idx_t n)
{
    /*  Below the offset the behaviour is unspecified. */
    if (n < 2)
        return (seq_t)0;

    /*  Ensure cache large enough for n. */
    if (n >= cache_cap) {
        /*  Grow to at least max( n+1 , 14 ) to have base values. */
        idx_t new_cap = n + 1;
        if (new_cap < 14)
            new_cap = 14;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /*  Initialise base values if the cache has just been created or
        enlarged beyond the current cached maximum. */
    if (cached_max < 2) {
        /*  Fill indices 0..1 with 0 (unused). */
        cache[0] = cache[1] = (seq_t)0;
        /*  Fill the initial array up to 13. */
        for (idx_t i = 2; i <= 13 && i < cache_cap; ++i)
            cache[i] = init_vals[i];
        cached_max = 13;
    } else if (cached_max < 13) {
        /*  Cache exists but is missing some of the initial terms. */
        for (idx_t i = cached_max + 1; i <= 13 && i < cache_cap; ++i)
            cache[i] = init_vals[i];
        if (cached_max < 13)
            cached_max = 13;
    }

    /*  Compute any missing terms up to n if needed. */
    if (n > cached_max) {
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if (i <= 13) {
                cache[i] = init_vals[i];
            } else {
                /*  Recurrence: a(n) = a(n-4) + a(n-6) - a(n-10) for n>13. */
                cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
            }
        }
        cached_max = n;
    }

    return cache[n];
}
