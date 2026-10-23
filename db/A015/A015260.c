/*
 * A015260  Gaussian binomial coefficient [ n,2 ] for q = -9.
 * https://oeis.org/A015260
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/800)*((-9)^n - 1)*(1 - (-9)^(n-1)) (recurrence form:
 *            a(2)=1, a(3)=73, a(4)=5986, a(n)=73*a(n-1)+657*a(n-2)-729*a(n-3)
 *            for n≥5)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 2; return 0 to avoid surprises. */
    if (n < 2) return 0;

    /* Cached values, indexed directly by n. */
    static seq_t *cache = NULL;          /* allocated at least n+1 elements */
    static idx_t  cache_len   = 0;       /* allocated length */
    static idx_t  max_computed = 0;      /* highest n already computed */

    /* Resize cache if needed. */
    if ((idx_t)n + 1 > cache_len)
    {
        idx_t new_len = (idx_t)n + 1;
        /* (void*) → seq_t* cast is legal in both C and C++ */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure */

        cache = tmp;
        /* zero the new portion */
        for (idx_t i = cache_len; i < new_len; ++i) cache[i] = 0;
        cache_len = new_len;

        /* initialise the first few terms */
        if (cache_len > 2) cache[2] = 1;
        if (cache_len > 3) cache[3] = 73;
        if (cache_len > 4) cache[4] = 5986;
    }

    /* Return if already computed. */
    if (n <= max_computed) return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        if (i == 2) { cache[i] = 1; continue; }
        if (i == 3) { cache[i] = 73; continue; }
        if (i == 4) { cache[i] = 5986; continue; }

        seq_t term = cache[i - 1];
        term *= 73;                        /* 73*a(i-1) */
        term += cache[i - 2] * 657;         /* +657*a(i-2) */
        term -= cache[i - 3] * 729;         /* -729*a(i-3) */
        cache[i] = term;
    }

    max_computed = n;
    return cache[n];
}
