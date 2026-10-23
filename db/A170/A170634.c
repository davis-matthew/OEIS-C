/*
 * A170634  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 47 * Σ_{i=1}^{47} a(n-i) - 1128 * a(n-48) * + c(n), where
 *            * c(0)=1, c(1..47)=2, c(48)=1, c(n)=0 otherwise
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Memoised cache of already computed values */
static seq_t *cache = NULL;      /* array of a(i) values       */
static idx_t  cache_cap = 0;     /* allocated capacity        */
static idx_t  cache_computed = 0;/* number of values filled   */

/* Resize the cache so that index n (inclusive) is valid */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    idx_t new_cap = (n + 1) * 2;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* allocation failure */
    if (cache) {
        memcpy(tmp, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n >= cache_computed) {
        ensure_cache(n);
        for (idx_t idx = cache_computed; idx <= n; ++idx) {
            /* coefficient from N(t) */
            seq_t coeff;
            if (idx == 0 || idx == 48) coeff = 1;
            else if (idx >= 1 && idx <= 47) coeff = 2;
            else coeff = 0;

            /* compute 47 * Σ_{i=1}^{47} a(idx-i) */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 47 && idx >= k; ++k)
                sum += cache[idx - k];

            seq_t term = 47 * sum;

            /* subtract 1128*a(idx-48) if idx>=48 */
            if (idx >= 48)
                term -= 1128 * cache[idx - 48];

            /* add constant coefficient */
            term += coeff;

            cache[idx] = term;
        }
        cache_computed = n + 1;
    }
    return cache[n];
}
