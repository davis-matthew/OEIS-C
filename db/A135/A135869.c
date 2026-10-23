/*
 * A135869  G.f. A(x) satisfies A(x) = 1 + x*A(3*x)^2.
 * https://oeis.org/A135869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 3^(n-1) * Sum_{k=0}^{n-1} a(k) * a(n-1-k) for
 *            n≥1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage: seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms and state for computing powers of 3. */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* Number of terms currently stored (>=1). */
    static seq_t  pow3_next = 1;   /* 3^(i-1) for the next term to compute (i = cache_len). */

    /* Lazily initialize the cache with a(0) = 1 and prepare for subsequent terms. */
    if (cache_len == 0)
    {
        cache_len = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        pow3_next = (seq_t)1;         /* for i = 1, we need 3^(1-1) = 1 */
    }

    /* Grow the cache if we need values beyond what we have computed. */
    if (n >= cache_len)
    {
        size_t newlen = (size_t)(n + 1);
        seq_t *newp = (seq_t*)realloc(cache, newlen * sizeof(seq_t));
        if (newp == NULL) /* allocation failure – terminate program */ abort();
        cache = newp;
        /* Initialize new elements to zero (just in case). */
        for (idx_t i = cache_len; i < newlen; ++i) cache[i] = (seq_t)0;

        /* Compute missing terms iteratively using the recurrence. */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k)
                sum += cache[k] * cache[i - 1 - k];   /* convolution for a(i) */

            cache[i] = pow3_next * sum;   /* a(i) = 3^(i-1) * sum */
            pow3_next *= (seq_t)3;        /* prepare power of 3 for the next index */
        }
        cache_len = n + 1;
    }

    return cache[n];
}
