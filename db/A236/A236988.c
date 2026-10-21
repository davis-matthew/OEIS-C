/*
 * A236988  Real part of the product of all the Gaussian integers in the
 *          rectangle [1, 1] to [2, n].
 * https://oeis.org/A236988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) satisfies
 *            a(n)+(2*n+3)*(n-2)*a(n-1)+n*(n+1)*(n^2-4*n+8)*a(n-2)
 *            -2*(n^2-4*n+8)*(n^2-4*n+5)*a(n-3)=0 with a(1)=1, a(2)=-20,
 *            a(3)=140
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 12, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t last_computed = 0;

    /* Initialise cache for at least indices 1..3 */
    if (cache == NULL) {
        cache_cap = (n > 3 ? n + 1 : 4);   /* allocate a bit more than needed */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-20;
        cache[3] = (seq_t)140;
        last_computed = 3;
    }

    /* Expand cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* Compute values up to n if they are missing */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* Compute terms using seq_t arithmetic */
            seq_t i_seq      = (seq_t)i;
            seq_t i2         = i_seq * i_seq;
            seq_t term234    = ((seq_t)(2*i + 3)) * ((seq_t)(i - 2));
            seq_t termA      = term234 * cache[i-1];

            seq_t t           = i2 - (seq_t)4*i + (seq_t)8;   /* n^2-4n+8 */
            seq_t termB       = i_seq * (i_seq + 1) * t * cache[i-2];

            seq_t t2          = t * ((seq_t)(i2 - 4*i + 5)); /* (n^2-4n+8)*(n^2-4n+5) */
            seq_t termC       = (seq_t)2 * t2 * cache[i-3];

            /* a(n) = -termA - termB + termC */
            cache[i] = -termA - termB + termC;
        }
        last_computed = n;
    }

    return cache[n];
}
