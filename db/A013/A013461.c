/*
 * A013461  Expansion of e.g.f. exp(arctan(x)-arcsinh(x)).
 * https://oeis.org/A013461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A013461: Expansion of e.g.f. exp(arctan(x)-arcsinh(x))
   Recurrence (for n ≥ 0):
      (n + 2)*(n – 1)*(n + 1)^2*a(n)
      – (n + 2)*(1 + 2*n)*a(n + 1)
      + (n + 2)*(2*n + 3)*a(n + 2)
      – 2*a(n + 3)
      + a(n + 4) = 0
   Initial values:
      a(0)=1, a(1)=0, a(2)=0, a(3)=-1, a(4)=0
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocation capacity */

    /* initialise cache with the first five terms on first call */
    if (cache_len == 0) {
        cache_cap = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;   /* a(0) */
        cache[1] = 0;   /* a(1) */
        cache[2] = 0;   /* a(2) */
        cache[3] = -1;  /* a(3) */
        cache[4] = 0;   /* a(4) */
        cache_len = 5;
    }

    /* return cached value if already computed */
    if (n < cache_len) return cache[n];

    /* enlarge cache if needed */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* the recurrence uses a(n), a(n+1), a(n+2), a(n+3) where
           n = i - 4 */
        idx_t idx_n = (idx_t)(i - 4);

        seq_t a_n  = cache[idx_n];
        seq_t a_n1 = cache[idx_n + 1];
        seq_t a_n2 = cache[idx_n + 2];
        seq_t a_n3 = cache[idx_n + 3];

        /* term1 = (n + 2)*(n – 1)*(n + 1)^2 * a(n) */
        seq_t term1 = (seq_t)(idx_n + 2) * (seq_t)(idx_n - 1) *
                      (seq_t)(idx_n + 1) * (seq_t)(idx_n + 1) * a_n;

        /* term2 = –(n + 2)*(1 + 2*n)*a(n + 1) */
        seq_t term2 = (seq_t)(idx_n + 2) * (seq_t)(1 + 2 * idx_n) * a_n1;
        term2 = -term2;

        /* term3 = (n + 2)*(2*n + 3)*a(n + 2) */
        seq_t term3 = (seq_t)(idx_n + 2) * (seq_t)(2 * idx_n + 3) * a_n2;

        /* term4 = –2*a(n + 3) */
        seq_t term4 = 2 * a_n3;
        term4 = -term4;

        seq_t sum = term1 + term2 + term3 + term4;
        cache[i] = -sum;   /* a(n + 4) = –sum */
    }

    cache_len = needed;
    return cache[n];
}
