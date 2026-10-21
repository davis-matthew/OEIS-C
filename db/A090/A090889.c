/*
 * A090889  Double partial sums of (n * its dyadic valuation).
 * https://oeis.org/A090889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = total of partial sums of k*v2(k) for k=0..n, i.e. a(n) =
 *            Σ_{i=0..n} Σ_{j=0..i} j*v2(j). Implements a memoised iterative
 *            algorithm with O(1) per term
 * Range:     u8 n <= 11, u16 n <= 72, u32 n <= 2953, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored so far */
    static idx_t cache_cap = 0;    /* allocated capacity */
    static seq_t curr_S = 0;       /* current partial sum S(i)=∑_{j=0..i} j*v2(j) */

    /* Ensure cache is large enough */
    if (n >= cache_cap) {
        idx_t new_cap = (n + 1) * 2;
        if (new_cap < n + 1) new_cap = n + 1; /* prevent overflow */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure; abort as build will fail */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize cache for n=0 if empty */
    if (cache_size == 0) {
        cache[0] = 0;
        cache_size = 1;
        curr_S = 0;
    }

    /* Extend cache up to required n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* compute v2(i) */
        idx_t tz = 0;
        if (i != 0) {
            idx_t temp = i;
            while ((temp & 1) == 0) {
                ++tz;
                temp >>= 1;
            }
        }
        /* f = i * v2(i)  (seq_t arithmetic) */
        seq_t f = (seq_t)i * (seq_t)tz;
        curr_S += f;                    /* update partial sum S(i) */
        seq_t prev = (i == 0) ? 0 : cache[i - 1];
        seq_t a_i = prev + curr_S;      /* a(i) = a(i-1) + S(i) */
        cache[i] = a_i;
    }

    cache_size = n + 1;
    return cache[n];
}
