/*
 * A188458  Expansion of e.g.f. exp(x)/cosh(2*x).
 * https://oeis.org/A188458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} -(-1)^(n*k)*C(n, k)*a(n-k) a(0) = 1
 *            Implements memoised calculation of a(n) for arbitrary-precision
 *            seq_t. The function is thread‑safe only if calls are sequential
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 31,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;      /* array of computed a(n) values   */
static idx_t  cache_len  = 0;    /* highest index + 1 stored       */
static idx_t  cache_cap  = 0;    /* current allocation capacity    */

static void grow_cache(idx_t new_cap)
{
    if (new_cap <= cache_cap) return;
    seq_t *new_cache = alloc_seq_array(new_cap);
    if (cache) {
        for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        free_seq_array(cache);
    }
    cache     = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Allocate / expand cache if necessary */
    if (n + 1 > cache_len) {
        grow_cache((n + 1) * 2);
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
                continue;
            }

            seq_t sum  = (seq_t)0;
            seq_t binom = (seq_t)i;          /* C(i,1) for first k=1 */

            for (idx_t k = 1; k <= i; ++k) {
                /* sign = -(-1)^(i*k)  */
                seq_t factor;
                if ((i & 1ULL) == 0) {         /* i even => sign = -1  */
                    factor = -(seq_t)1;
                } else {                       /* i odd  */
                    if ((k & 1ULL) == 0)       /* k even => sign = -1 */
                        factor = -(seq_t)1;
                    else                       /* k odd  => sign = +1 */
                        factor = +(seq_t)1;
                }

                seq_t term = binom * cache[i - k];
                if (factor < 0) term = -term;
                sum = sum + term;

                /* update binom for next k: C(i,k+1) = C(i,k) * (i-k)/(k+1) */
                if (k < i) {
                    seq_t numerator = binom * (seq_t)(i - k);
                    binom = numerator / (seq_t)(k + 1);
                }
            }
            cache[i] = sum;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
