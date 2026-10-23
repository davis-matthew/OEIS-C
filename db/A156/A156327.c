/*
 * A156327  E.g.f.: A(x) = exp( Sum_{n>=1} n*(n+3)/2 * a(n-1)*x^n/n! ) =
 *          Sum_{n>=0} a(n)*x^n/n! with a(0)=1.
 * https://oeis.org/A156327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binom_inc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A156327: a(0)=1
 * Recurrence: a(n) = Σ_{k=1..n} k*(k+3)/2 * C(n-1,k-1) * a(k-1) * a(n-k)
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "../utils/binom_inc.h"

static seq_t *a_cache = NULL;  /* memoization array */
static idx_t cache_size = 0;   /* number of entries currently stored (>=0) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Resize cache if necessary */
    if ((idx_t)cache_size <= n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0)
            abort();   /* out of memory */
        a_cache = tmp;
        if (cache_size == 0) {
            /* initialise first term */
            a_cache[0] = (seq_t)1;
            cache_size = 1;
        }
    }

    /* Compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;  /* C(i-1,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* w_k = k*(k+3)/2 */
            seq_t w = (seq_t)k;
            w *= (k + 3);
            w /= 2;

            seq_t term = w;
            term *= binom;
            term *= a_cache[k - 1];
            term *= a_cache[i - k];
            sum += term;

            /* update binom for next k: C(i-1,k) from C(i-1,k-1) */
            if (k < i) {
                binom *= (seq_t)(i - k);
                binom /= (seq_t)k;
            }
        }

        a_cache[i] = sum;
    }

    cache_size = n + 1;
    return a_cache[n];
}
