/*
 * A166368  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*∑_{j=1..10} a(n-j) – 36*a(n-11) + pₙ where p₀ = 1,
 *            p₁…p₁₀ = 2, p₁₁ = 1, and pₙ = 0 for n ≥ 12. Initial values:
 *            a(0)=1, a(1)=10, a(n)=9*a(n‑1) for 2 ≤ n ≤ 10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization (0‑based indexing). */
    static seq_t *cache   = NULL; /* dynamic array holding computed values */
    static size_t cache_len = 0;  /* number of entries currently stored */

    /* Expand cache to at least (n+1) entries if needed. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_len)
    {
        /* Allocate or grow the array. */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failure – return 0 as fallback */
        cache = tmp;

        /* Compute new entries in order.  Use size_t for indices. */
        for (size_t i = cache_len; i < needed; ++i)
        {
            if (i == 0)                         /* a(0) */
                cache[i] = (seq_t)1;
            else if (i == 1)                    /* a(1) */
                cache[i] = (seq_t)10;
            else if (i <= 10)                   /* 2 ≤ n ≤ 10 : 9‑fold growth */
                cache[i] = cache[i - 1] * (seq_t)9;
            else                                /* recurrence for n ≥ 11 */
            {
                seq_t sum = 0;
                for (size_t j = 1; j <= 10; ++j)
                    sum += cache[i - j];

                seq_t val = (seq_t)8 * sum - (seq_t)36 * cache[i - 11];

                /* Add the numerator correction pₙ if n == 11. */
                if (i == 11)
                    val += (seq_t)1;

                cache[i] = val;
            }
        }

        cache_len = needed;
    }

    return cache[(size_t)n];
}
