/*
 * A144371  Shifts 7 places left under Dirichlet convolution.
 * https://oeis.org/A144371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   a(n) = 1 for n ≤ 7 * a(n) = Σ_{d|(n‑7)} a(d)∙a((n‑7)/d) for n ≥
 *            8
 * Range:     u8 n <= 50, u16 n <= 105, u32 n <= 215, u64 n <= 439,
 *            u128 n <= 887, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* internal cache for memoisation */
static seq_t *cache = NULL;     /* array of a(k) values, 1‑indexed (stored at index k-1) */
static idx_t  cache_size = 0;   /* number of terms that are already computed */
static size_t cache_cap   = 0;   /* allocated capacity of the array */

/* extend the cache so that it contains values up to (and including) 'up' */
static void compute_up_to(idx_t up)
{
    if (up <= cache_size)
        return;                 /* already computed */

    /* enlarge capacity if necessary */
    if (up > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap < (size_t)up)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t n = cache_size + 1; n <= up; ++n) {
        if (n <= 7) {
            cache[n - 1] = (seq_t)1;
        } else {
            idx_t k = n - 7;          /* argument of the Dirichlet convolution */
            seq_t sum = (seq_t)0;
            /* enumerate divisors of k */
            for (idx_t d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    idx_t e = k / d;
                    seq_t prod = cache[d - 1] * cache[e - 1];
                    sum += prod;
                    if (d != e)   /* pair divisor contributes twice */
                        sum += prod;
                }
            }
            cache[n - 1] = sum;
        }
    }
    cache_size = up;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* offset is 1 */
    compute_up_to(n);
    return cache[n - 1];
}
