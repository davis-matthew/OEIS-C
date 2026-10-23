/*
 * A191277  Expansion of e.g.f. 1/(1 - sinh(x)*cosh(x)).
 * https://oeis.org/A191277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..⌊(n-1)/2⌋} 4^k * binomial(n,2k+1) * a(n-2k-1)
 *            with a(0) = 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_seq.h"
#include "../utils/binomial.h"

/* External function as required */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* currently allocated size */
    static idx_t  cache_max = (idx_t)(-1);  /* highest index computed */

    /* Ensure the cache can hold indices up to n */
    if (n >= cache_cap) {
        /* grow_seq will allocate (if necessary) and set cache_cap appropriately */
        grow_seq(&cache, &cache_cap, (size_t)n + 1);
    }

    /* Compute missing values from cache_max+1 up to n */
    /* Handle the special case for a(0) */
    if (cache_max < 0) {
        cache[0] = 1;
        cache_max = 0;
    }

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 0) {           /* already handled, but keep for safety */
            cache[0] = 1;
            continue;
        }

        seq_t sum = 0;                       /* running total */
        idx_t max_k = (i - 1) / 2;            /* floor((i-1)/2) */
        seq_t pow4 = 1;                       /* 4^k, starts with k=0 */
        idx_t j = 1;                          /* current odd index (2*k+1) */

        /* Start with binomial(i,1) */
        seq_t binom = binomial(i, j);

        for (idx_t k = 0; k <= max_k; ++k) {
            /* term = binom * pow4 * cache[i-j] */
            seq_t term = binom;
            term *= pow4;
            term *= cache[i - j];
            sum += term;

            if (k < max_k) {
                /* update binom to binomial(i, j+2) */
                seq_t mult1 = (seq_t)(i - j);
                seq_t mult2 = (seq_t)(i - j - 1);
                seq_t denom1 = (seq_t)(j + 1);
                seq_t denom2 = (seq_t)(j + 2);

                binom *= mult1;
                binom *= mult2;
                binom /= denom1;
                binom /= denom2;

                /* update power of 4 */
                pow4 *= 4;

                j += 2; /* next odd index */
            }
        }

        cache[i] = sum;
    }

    cache_max = n;
    return cache[n];
}
