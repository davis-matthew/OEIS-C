/*
 * A116873  Generalized Catalan numbers C(2,3;n) = C(3,2;n).
 * https://oeis.org/A116873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=5, and for n≥3 a(n) = (4*(5*n-14)*a(n-1) +
 *            3*(31*n-79)*a(n-2) + 36*(2*n-5)*a(n-3)) / (n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Derived from (1‑n)*a(n)+4*(5*n-14)*a(n‑1)+3*(31*n-79)*a(n‑2)+36*(2*n-5)*a(n‑3)=0) */

#include <stdlib.h>

/* A larger type for intermediate arithmetic.  If the compiler supports
   unsigned __int128 we use it; otherwise we fall back to unsigned long long. */
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 big_t;
#else
typedef unsigned long long big_t;
#endif

/* Memoised evaluation of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of already computed values      */
    static idx_t  size  = 0;      /* number of valid entries in cache      */

    /* Grow the cache if necessary. */
    if (n >= size) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
        cache = tmp;

        /* Compute the missing values.  The loop runs from the current size
           (the first uninitialised index) up to and including n. */
        for (idx_t i = size; i <= n; ++i) {
            if (i == 0 || i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)5;
            } else {
                /* Use a larger intermediate type to avoid overflow of
                   the products.  The factor (n‑1) is always positive for i≥3,
                   so integer division yields the correct result.          */
                big_t c1 = (big_t)(5*i - 14);
                big_t c2 = (big_t)(31*i - 79);
                big_t c3 = (big_t)(2*i  - 5);

                big_t term1 = 4u * c1 * (big_t)cache[i-1];
                big_t term2 = 3u * c2 * (big_t)cache[i-2];
                big_t term3 = 36u * c3 * (big_t)cache[i-3];

                big_t sum = term1 + term2 + term3;
                cache[i] = (seq_t)(sum / (big_t)(i-1));
            }
        }
        size = n + 1;
    }

    return cache[n];
}
