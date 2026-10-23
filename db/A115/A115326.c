/*
 * A115326  E.g.f.: exp(x/(1-2*x))/sqrt(1-4*x^2).
 * https://oeis.org/A115326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1)*a(n-1) + 2*(n-1)*(2*n-1)*a(n-2) -
 *            8*(n-2)^2*(n-1)*a(n-3), with a(0)=1, a(1)=1, a(2)=9
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with first three terms */
    if (cache == NULL) {
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        if (cache == NULL) return 0;  /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)9;
        cache_size = 3;
    }

    /* extend cache if we need more terms */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;  /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 3) continue;  /* first three terms already set */

            seq_t a1 = cache[i - 1];  /* a(n-1) */
            seq_t a2 = cache[i - 2];  /* a(n-2) */
            seq_t a3 = cache[i - 3];  /* a(n-3) */

            /* compute the factors using idx_t arithmetic, then cast */
            idx_t two_i_minus1 = 2 * i - 1;
            seq_t factor1 = (seq_t)two_i_minus1;

            idx_t temp = (i - 1) * two_i_minus1;
            seq_t factor3 = (seq_t)(2 * temp);

            idx_t temp2 = (i - 2) * (i - 2) * (i - 1);
            seq_t factor4 = (seq_t)(8 * temp2);

            seq_t term = factor1 * a1 + factor3 * a2 - factor4 * a3;
            cache[i] = term;
        }

        cache_size = new_size;
    }

    return cache[n];
}
