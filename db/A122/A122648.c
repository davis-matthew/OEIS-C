/*
 * A122648  Expansion of e.g.f.: exp(x^2)*(exp(2*x)+1)/2.
 * https://oeis.org/A122648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence a(n) - 2*a(n-1) +
 *            2*(-2*n+3)*a(n-2) + 4*(n-2)*a(n-3) + 4*(n-2)*(n-3)*a(n-4) = 0
 *            with initial values a(0)=1, a(1)=1, a(2)=4, a(3)=10. The
 *            function below computes a(n) for any n≥0 using memoised
 *            recurrence
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation table */
    static seq_t *cache = NULL;   /* array of computed a(i) */
    static idx_t cache_len = 0;   /* number of entries stored (cache_len–1 is last valid index) */

    if (n >= cache_len) {
        /* grow cache to size n+1 */
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* fill newly allocated part */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0)          cache[i] = (seq_t)1;
            else if (i == 1)     cache[i] = (seq_t)1;
            else if (i == 2)     cache[i] = (seq_t)4;
            else if (i == 3)     cache[i] = (seq_t)10;
            else {
                /* positive part: 2*a[i-1] + 2*(2*i-3)*a[i-2] */
                seq_t pos = 2 * cache[i-1];
                seq_t term = (seq_t)(2*i - 3);
                term = term * cache[i-2];
                term = 2 * term;
                pos += term;

                /* negative part: 4*(i-2)*a[i-3] + 4*(i-2)*(i-3)*a[i-4] */
                seq_t neg = 4 * ((seq_t)(i-2) * cache[i-3]);
                seq_t term2 = (seq_t)(i-2) * (i-3);
                term2 = term2 * cache[i-4];
                term2 = 4 * term2;
                neg += term2;

                cache[i] = pos - neg;   /* pos ≥ neg for all n by recurrence */
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
