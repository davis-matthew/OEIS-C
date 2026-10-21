/*
 * A097679  E.g.f.: (1/(1-x^4))*exp( 4*Sum_{i>=0} x^(4*i+1)/(4*i+1) ) for an
 *          order-4 linear recurrence with varying coefficients.
 * https://oeis.org/A097679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For n < 4, a(n) = 4^n. For n >= 4, a(n) = 4*a(n-1) +
 *            (n!/(n-4)!)*a(n-4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache for computed terms */
static idx_t cache_len = 0;   /* number of terms currently cached (0..cache_len-1) */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold term n. */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i < 4) {
                /* a(i) = 4^i */
                seq_t val = 1;
                for (idx_t j = 0; j < i; ++j)
                    val *= 4;
                cache[i] = val;
            } else {
                /* a(i) = 4*a(i-1) + (i*(i-1)*(i-2)*(i-3))*a(i-4) */
                seq_t term1 = 4 * cache[i - 1];
                seq_t coeff = (seq_t)i * (seq_t)(i - 1) *
                              (seq_t)(i - 2) * (seq_t)(i - 3);
                cache[i] = term1 + coeff * cache[i - 4];
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
