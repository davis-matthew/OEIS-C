/*
 * A212857  Number of 4 X n arrays with rows being permutations of 0..n-1 and
 *          no column j greater than column j-1 in all rows.
 * https://oeis.org/A212857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^(n-1) + Sum_{s=1..n-1} a(s) * (-1)^(n-s-1) *
 *            binomial(n,s)^4 Initial values: a(0)=1, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required headers for dynamic allocation */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache  = NULL;
    static idx_t  size   = 0;

    /* initialise cache for a(0) and a(1) if needed */
    if (cache == NULL) {
        size = 2;
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* if requested value already cached, return immediately */
    if (n < size) return cache[n];

    /* keep the current cache size to know which indices still need
       to be computed.  Reallocate cache first if necessary, but do
       not change `size` until after the loop. */
    idx_t start = size;
    if (n >= size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        size  = new_size;
    }

    /* compute a(k) for all k from start to n (inclusive) */
    for (idx_t k = start; k <= n; ++k) {
        /* k >= 2 because start > 1 for new indices */
        seq_t sum = (seq_t)0;

        /* binomial coefficient C(k,1) = k */
        seq_t binom = (seq_t)k;

        for (idx_t s = 1; s <= k - 1; ++s) {
            /* compute binom^4 */
            seq_t pow2 = binom * binom;      /* binom^2 */
            seq_t pow4 = pow2 * pow2;        /* binom^4 */

            /* term = a(s) * binom^4 */
            seq_t term = cache[s] * pow4;

            /* add or subtract term depending on parity of (k-s-1) */
            if (((k - s - 1) & 1) == 0) {
                sum += term;                  /* positive sign */
            } else {
                sum -= term;                  /* negative sign */
            }

            /* update binom to C(k, s+1) for next iteration */
            if (s < k - 1) {
                binom = binom * (seq_t)(k - s) / (seq_t)(s + 1);
            }
        }

        /* add the initial (-1)^(k-1) term */
        if (((k - 1) & 1) == 0) {
            sum += (seq_t)1;
        } else {
            sum -= (seq_t)1;
        }

        cache[k] = sum;
    }

    return cache[n];
}
