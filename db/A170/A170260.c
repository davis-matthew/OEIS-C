/*
 * A170260  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n + 9 * Σ_{i=1}^{min(40,n)} a(n-i) - 45 * a(n-41) where
 *            p_0 = 1, p_{1..40} = 2, p_41 = 1, and p_{>41} = 0. This follows
 *            from the rational generating function (1 + 2 t + … + 2 t^40 +
 *            t^41) / (1 - 9 t - 9 t^2 - … - 9 t^40 + 45 t^41)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values */
static size_t cap    = 0;        /* current allocation length   */
static size_t computed = 0;      /* number of entries already computed */

/* Ensure the cache can hold values up to index 'atLeast-1'. */
static void ensure_cap(size_t atLeast)
{
    if (atLeast <= cap) return;
    size_t newCap = (cap > 0 ? cap << 1 : 1);
    while (newCap < atLeast) newCap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, newCap * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failed */
    cache = tmp;
    cap   = newCap;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;

    /* Compute missing values up to N. */
    if (computed <= N) {
        ensure_cap(N + 1);
        for (size_t k = computed; k <= N; ++k) {
            seq_t sum = 0;
            size_t d   = (k <= 40 ? k : 40);
            for (size_t i = 1; i <= d; ++i)
                sum += cache[k - i];

            seq_t pk = 0;
            if (k == 0)          pk = 1;
            else if (k <= 40)    pk = 2;
            else if (k == 41)    pk = 1;
            /* else pk remains 0 for k > 41 */

            seq_t val = pk;
            val += (seq_t)9 * sum;
            if (k >= 41)
                val -= (seq_t)45 * cache[k - 41];

            cache[k] = val;
        }
        computed = N + 1;
    }
    return cache[N];
}
