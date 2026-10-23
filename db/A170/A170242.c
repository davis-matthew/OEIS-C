/*
 * A170242  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coeff(n) + 39 * Σ_{k=1}^{39} a(n-k) - 780 * a(n-40) where
 *            coeff(n) = 1 for n==0 or n==40, coeff(n) = 2 for 1≤n≤39,
 *            coeff(n) = 0 otherwise. This recurrence follows from the
 *            rational generating function (t^40 + 2t^39 + … + 2t +
 *            1)/(780t^40 - 39t^39 - … - 39t + 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

extern seq_t a(idx_t n);

static seq_t *cache = NULL;
static idx_t cache_len = 0;     /* number of elements actually stored */
static size_t cache_cap = 0;    /* allocated capacity */

/* Helper: ensures cache capacity >= need. */
static void ensure_capacity(idx_t need)
{
    if ((size_t)need + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if ((size_t)need + 1 > new_cap)
            new_cap = (size_t)need + 1;
        seq_t *tmp = (cache == NULL) ? (seq_t *)malloc(new_cap * sizeof(seq_t))
                                     : (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* extend cache up to n if necessary */
    if (n >= cache_len) {
        ensure_capacity(n);
        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;

            /* sum_{k=1}^{min(39,i)} 39 * a(i-k) */
            idx_t maxk = (i >= 39) ? 39 : (idx_t)i;
            for (idx_t k = 1; k <= maxk; ++k)
                sum += (seq_t)39 * cache[i - k];

            /* subtract 780 * a(i-40) if applicable */
            if (i >= 40)
                sum -= (seq_t)780 * cache[i - 40];

            /* add coeff(i) */
            seq_t coeff = 0;
            if (i == 0 || i == 40)
                coeff = 1;
            else if (i >= 1 && i <= 39)
                coeff = 2;

            cache[i] = coeff + sum;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
