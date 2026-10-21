/*
 * A080635  Number of permutations on n letters without double falls and
 *          without initial falls.
 * https://oeis.org/A080635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n≥0
   a(n+1) = (∑_{k=0..n} C(n,k) * a(k) * a(n-k)) - a(n) + (n==0?1:0)
   where C(n,k) is the binomial coefficient. */

#include <stdlib.h>
#include <string.h>

/* Memoised table of previously computed terms.
   The table grows on demand. */
static seq_t *cache         = NULL;      /* array of a(0), a(1), … */
static size_t cache_cap     = 0;         /* allocated capacity (≥ cache_max+1) */
static idx_t  cache_max     = (idx_t)-1;  /* highest index computed so far */

/* Compute binomial(n,k) as seq_t, using multiplicative formula.
   Assumes 0 ≤ k ≤ n. */
static seq_t binom_idx(idx_t n, idx_t k) {
    seq_t res = (seq_t)1;
    if (k > n / 2) k = n - k;   /* use symmetry for fewer steps */
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();        /* allocation failure */
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        cache_max = 0;
    }

    /* Compute missing terms up to n */
    if ((idx_t)cache_max < n) {
        /* Grow capacity if necessary */
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        if (new_cap != cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute a(k) for k = cache_max+1 … n
           Using the recurrence with nprev = k-1 */
        for (idx_t k = cache_max + 1; k <= n; ++k) {
            idx_t nprev = k - 1;
            seq_t sum = (seq_t)0;

            /* convolution sum */
            for (idx_t j = 0; j <= nprev; ++j) {
                seq_t bin = binom_idx(nprev, j);
                sum += bin * cache[j] * cache[nprev - j];
            }

            /* add 0^nprev when nprev==0 (i.e., k==1) */
            if (nprev == 0) sum += (seq_t)1;

            /* recurrence: a(k) = sum - a(nprev) */
            cache[k] = sum - cache[nprev];
        }
        cache_max = n;
    }

    return cache[n];
}
