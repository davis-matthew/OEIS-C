/*
 * A168770  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n ≥ 1, a(n) = 43*(a(n-1)+...+a(n-17)) –
 *            946*a(n-18) + p(n), where p(n) = 1 for n = 0 or 18, p(n) = 2 for
 *            1 ≤ n ≤ 17, p(n) = 0 otherwise. The recurrence is obtained from
 *            the rational generating function P(t)/Q(t) with P(t) = 1 + 2t +
 *            … + 2t^17 + t^18 and Q(t) = 1 – 43t – … – 43t^17 +...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;          /* cache[i] = a(i) */
static idx_t cache_size = 0;         /* allocated capacity of cache */
static idx_t cached_upto = (idx_t)-1;/* largest index computed; (idx_t)-1 = not yet */

/* Ensure cache has room for indices 0..n */
static void
ensure_cache_size(idx_t n)
{
    if (n + 1 <= cache_size)
        return;

    size_t new_cap = cache_size ? cache_size << 1 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* allocation failure */

    cache = tmp;
    cache_size = (idx_t)new_cap;
}

/* Compute a(n) */
seq_t
a(idx_t n)
{
    ensure_cache_size(n);

    /* initialize first term if needed */
    if (cached_upto == (idx_t)-1) {
        cache[0] = 1;
        cached_upto = 0;
    }

    /* compute missing terms up to n */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        seq_t val = 0;

        /* 43 * sum_{k=1}^{17} a(i-k) */
        for (idx_t k = 1; k <= 17 && i >= k; ++k) {
            val += 43 * cache[i - k];
        }

        /* – 946 * a(i-18) if applicable */
        if (i >= 18)
            val -= 946 * cache[i - 18];

        /* add the numerator contribution p(i) */
        if (i <= 17)
            val += 2;
        else if (i == 18)
            val += 1;
        /* p(i) = 0 for i > 18 */

        cache[i] = val;
    }

    cached_upto = n;
    return cache[n];
}
