/*
 * A025991  Expansion of 1/((1-2*x)*(1-5*x)*(1-6*x)*(1-12*x)).
 * https://oeis.org/A025991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-1) - 208*a(n-2) + 684*a(n-3) - 720*a(n-4) with
 *            a(0)=1, a(1)=25, a(2)=417, a(3)=5909
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
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

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0) abort();   /* allocation failure */
        cache = tmp;

        /* Compute new elements up to n (inclusive) */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0)
                cache[0] = (seq_t)1;
            else if (i == 1)
                cache[1] = (seq_t)25;
            else if (i == 2)
                cache[2] = (seq_t)417;
            else if (i == 3)
                cache[3] = (seq_t)5909;
            else {
                seq_t term1 = cache[i-1] * (seq_t)25;
                seq_t term2 = cache[i-2] * (seq_t)208;
                seq_t term3 = cache[i-3] * (seq_t)684;
                seq_t term4 = cache[i-4] * (seq_t)720;
                seq_t val = term1 + term3;
                val -= term2 + term4;
                cache[i] = val;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
