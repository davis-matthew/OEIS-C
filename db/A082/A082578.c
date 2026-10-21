/*
 * A082578  a(n) = Sum_{k=0..n} binomial(2*k, k) * binomial(2*k+1,k).
 * https://oeis.org/A082578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∑_{k=0..n} binomial(2k,k) * binomial(2k+1,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial helper is provided by the utility library. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Persistent cache of computed partial sums. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Grow the cache if it does not yet cover the requested index. */
    if (n >= cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)      /* allocation failure – leave the caller to handle it */
            return 0;
        cache = tmp;

        /* Compute all new terms up to n, extending the cumulative sum. */
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t term = binomial(2 * k, k);          /* C(2k,k)           */
            term = term * binomial(2 * k + 1, k);     /* * C(2k+1,k)       */
            if (k == 0)
                cache[0] = term;                      /* a(0) = 1           */
            else
                cache[k] = cache[k - 1] + term;        /* a(k) = a(k-1)+term */
        }
        cache_len = new_len;
    }
    return cache[n];
}
