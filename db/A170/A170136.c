/*
 * A170136  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170136
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

/*
 * Formula: a(0)=1.
 * For n>0:
 *   let p(n) = { 1 when n=0 or n=38,
 *                2 when 1<=n<=37,
 *                0 otherwise }
 *   a(n) = p(n) + 29 * Σ_{k=1}^{min(37,n)} a(n-k) - 435 * a(n-38)   (a(m)=0 for m<0)
 *
 * This follows directly from the generating function
 *   G(t) = (t^38 + 2t^37 + … + 2t + 1) / (435t^38 – 29t^37 – … – 29t + 1).
 */

#include <stdlib.h>

/* grow the cached array to hold indices up to n (inclusive) */
static void ensure_cache(idx_t n, seq_t **cache, size_t *cap)
{
    size_t needed = n + 1;
    if (*cap >= needed) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 64;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t*)realloc(*cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    *cache = tmp;
    *cap   = new_cap;
}

/* cache for previously computed values */
static seq_t *cache   = NULL;
static size_t cache_cap   = 0;
static idx_t  cache_size   = 0;   /* number of elements actually computed  (last index + 1) */

/* main OEIS sequence function */
seq_t a(idx_t n)
{
    /* ensure memory is large enough to store a[0] … a[n] */
    ensure_cache(n, &cache, &cache_cap);

    /* compute any missing values */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        /* compute the sum of the last up to 37 terms */
        seq_t sum = 0;
        idx_t maxk = (i < 37) ? i : 37;
        for (idx_t k = 1; k <= maxk; ++k)
            sum += cache[i - k];

        seq_t val = sum * (seq_t)29;

        /* add the appropriate p(i) term */
        if (i == 0 || i == 38)           val += (seq_t)1;
        else if (i <= 37)                val += (seq_t)2;
        /* otherwise p(i) = 0, nothing to add */

        /* subtract 435 * a(i-38) if i >= 38 */
        if (i >= 38)
            val -= (seq_t)435 * cache[i - 38];

        cache[i] = val;
    }

    cache_size = n + 1;   /* update the number of computed elements */
    return cache[n];
}
