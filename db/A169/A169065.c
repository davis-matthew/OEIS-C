/*
 * A169065  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, for n>=1 a(n)=nval + 2*∑_{k=1}^{min(24,n)}a(n-k) * -
 *            (n>=25 ? 3*a(n-25) : 0) * * where * nval = 1 (n=0) * nval = 2 (1
 *            ≤ n ≤ 24) * nval = 1 (n = 25) * nval = 0 (n > 25) * (This is
 *            equivalent to the recurrence derived from the rational *
 *            generating function of A169065 and avoids negative in...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* cached sequence values */
static seq_t *cache   = NULL;
static size_t cache_sz = 0;

/* ensure values a[0..n] are computed and stored in cache */
static void ensure_cache(idx_t n)
{
    size_t idx = (size_t)n;
    if (idx < cache_sz) return;          /* already available */

    /* initialise cache if first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) abort();      /* allocation failed */
        cache[0] = (seq_t)1;             /* a(0) = 1 */
        cache_sz = 1;
    }

    /* grow cache if needed */
    if (idx + 1 > cache_sz) {
        seq_t *tmp = (seq_t *)realloc(cache, (idx + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
    }

    /* compute new values */
    for (size_t i = cache_sz; i <= idx; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;              /* a(0) = 1 */
        } else {
            /* nval */
            seq_t n_val;
            if (i <= 24)       n_val = (seq_t)2;
            else if (i == 25)  n_val = (seq_t)1;
            else              n_val = (seq_t)0;

            /* sum of up to 24 previous terms */
            seq_t sum_prev = (seq_t)0;
            size_t kmax = (i < 24 ? i : 24);
            for (size_t k = 1; k <= kmax; ++k) {
                sum_prev += cache[i - k];
            }

            /* compute value */
            val = n_val + (sum_prev * (seq_t)2);
            if (i >= 25) {
                val -= (seq_t)3 * cache[i - 25];
            }
        }
        cache[i] = val;
    }
    cache_sz = idx + 1;
}

/* public API */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
