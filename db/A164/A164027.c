/*
 * A164027  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Formula (OEIS A164027):                                  *
 *   Denominator: 406*t^6 - 28*t^5 - 28*t^4 - 28*t^3 - 28*t^2 - 28*t + 1   *
 *   Numerator:   t^6 + 2*t^5 + 2*t^4 + 2*t^3 + 2*t^2 + 2*t + 1           *
 *   Recurrence for n >= 6:                                        *
 *     a(n) = 28*∑_{k=1}^5 a(n-k) - 406*a(n-6)                       *
 *     plus a constant R_n : R_6 = +1, all other R_n = 0.            *
 *   Initial values (offset 0):                                    *
 *     a(0)=1, a(1)=30, a(2)=870, a(3)=25230, a(4)=731670, a(5)=21218430 */

/* Memoisation cache */
static seq_t *cache         = NULL;   /* array of pre‑computed values   */
static idx_t  cache_cap     = 0;      /* allocated capacity            */
static idx_t  cached_up_to  = (idx_t)-1; /* last index that has a value */

/* Ensure the cache can hold indices up to n.  */
static void grow_cache(idx_t n)
{
    if (cache_cap > n) return;
    idx_t new_cap = (n + 1) * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initial six values for 0 <= n < 6.  */
static const seq_t init_vals[6] = {
    (seq_t)1,          /* a(0) */
    (seq_t)30,         /* a(1) */
    (seq_t)870,        /* a(2) */
    (seq_t)25230,      /* a(3) */
    (seq_t)731670,     /* a(4) */
    (seq_t)21218430    /* a(5) */
};

/* Sequence function (external linkage).  */
seq_t a(idx_t n)
{
    /* Return directly for the known first six terms.  */
    if (n < 6) return init_vals[n];

    /* Initialise the cache on the first invocation. */
    if (cache == NULL) {
        grow_cache(15);                     /* pre‑allocate a few more slots */
        for (idx_t i = 0; i < 6; ++i)       /* store the six initial values */
            cache[i] = init_vals[i];
        cached_up_to = 5;
    }

    /* Return cached value if already computed. */
    if (n <= cached_up_to) return cache[n];

    /* Ensure capacity for indices up to n. */
    grow_cache(n);

    /* Compute missing terms in order.  */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        /* sum of the last five terms */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= 5; ++k)
            sum += cache[i - k];

        /* main recurrence part */
        seq_t term = sum * (seq_t)28 - cache[i - 6] * (seq_t)406;

        /* add the numerator constant R_i (only for i==6).  */
        if (i == 6) term += (seq_t)1;

        cache[i] = term;
    }

    cached_up_to = n;
    return cache[n];
}
