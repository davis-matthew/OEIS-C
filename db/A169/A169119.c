/*
 * A169119  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) =
 *            8·(a(n-1)+a(n-2)+⋯+a(n-25)) – 36·a(n-26) with initial values
 *            a(0)=1, a(n) = 0 for n<0. The first 26 values are obtained from
 *            the rational generating function given in the OEIS entry
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    /* static data to store the computed sequence and its state */
    static bool init_done = false;
    static seq_t init[26];          /* a(0) … a(25) */
    static idx_t  cache_cap = 0;    /* capacity of the cache array */
    static idx_t  cache_size = 0;   /* number of values stored (max index) */
    static seq_t *cache = NULL;     /* array of previously computed values */

    /* -------------------------------------------------------------
       First call – initialise the first 26 terms from the G(f)
       ------------------------------------------------------------- */
    if (!init_done)
    {
        /* c[i] coefficients of the denominator Q(t):
           Q(t)=1-8*t-⋯-8*t^25+36*t^26   (indices 1…26) */
        seq_t c[27] = {0};
        for (idx_t i = 1; i <= 25; ++i) c[i] = (seq_t)8;
        c[26] = (seq_t)-36;          /* only used for 1/ Q(t) coefficients >25 */

        /* compute coefficients of 1 / Q(t) up to t^25  ->  b[0..25]  */
        seq_t b[26];
        b[0] = (seq_t)1;
        for (idx_t m = 1; m <= 25; ++m)
        {
            seq_t sum = 0;
            for (idx_t k = 0; k < m; ++k) sum += b[k];
            b[m] = sum * (seq_t)8;     /* c[1..25] are all 8 */
        }

        /* P(t) = t^26 + 2*(t^25+⋯+t)+1  ->  P_i as described   */
        for (idx_t i = 0; i <= 25; ++i)
        {
            seq_t val = 0;
            idx_t imax = (i <= 26) ? i : 26;
            for (idx_t j = 0; j <= imax; ++j)
            {
                seq_t pj;
                if (j == 0 || j == 26) pj = (seq_t)1;
                else                   pj = (seq_t)2;
                val += pj * b[i - j];
            }
            init[i] = val;
        }
        init_done = true;
    }

    /* -------------------------------------------------------------
       Create/cache the dynamic array to hold all computed terms.
       ------------------------------------------------------------- */
    if (!cache)
    {
        cache_cap = 26;
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i <= 25; ++i) cache[i] = init[i];
        cache_size = 25;    /* last index that has a value */
    }

    /* -------------------------------------------------------------
       Extend the cache if the requested n is beyond what we have
       ------------------------------------------------------------- */
    if (n > cache_size)
    {
        idx_t new_cap = n + 1;
        if (new_cap > cache_cap)
        {
            cache_cap = new_cap;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }

        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            /* sum of the previous 25 values */
            seq_t sum = 0;
            for (idx_t j = 1; j <= 25; ++j) sum += cache[i - j];

            /* recurrence: a(n) = 8*sum - 36*a(n-26) */
            cache[i] = sum * (seq_t)8 - cache[i - 26] * (seq_t)36;
        }
        cache_size = n;
    }

    return cache[(size_t)n];
}
