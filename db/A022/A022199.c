/*
 * A022199  Gaussian binomial coefficients [ n,8 ] for q = 3.
 * https://oeis.org/A022199
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..8} (3^(n-i+1)-1)/(3^i-1) Recurrence used
 *            for fast integer computation: a(8) = 1 a(n) = a(n-1) * (3^n - 1)
 *            / (3^(n-8) - 1) for n > 8 Since the ratio is always an integer,
 *            all operations are exact
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 10, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute 3^e as seq_t (exponentiation by squaring).  */
static seq_t pow3(idx_t e)
{
    seq_t result = 1;
    seq_t base   = 3;
    while (e)
    {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* Cached sequence values: cache[0] corresponds to n = 8.              */
static seq_t *cache      = NULL;     /* pointer to cached values       */
static idx_t  cache_size = 0;       /* allocated number of slots     */
static idx_t  cache_max  = 7;       /* highest n whose value is cached */

/* Return a(n) for n >= 8; 0 for n < 8.  */
seq_t a(idx_t n)
{
    if (n < 8)
        return 0;   /* undefined below offset */

    /* Initialise cache on first call */
    if (cache == NULL)
    {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failure – return 0 */
        cache[0] = (seq_t)1;   /* a(8) = 1 */
        cache_max = 8;
    }

    /* Return cached value if already computed */
    if (n <= cache_max)
        return cache[n - 8];

    /* Extend cache up to the requested n */
    for (idx_t cur = cache_max + 1; cur <= n; ++cur)
    {
        /* Compute recurrence factor */
        seq_t numerator   = pow3(cur);
        numerator -= 1;
        seq_t denominator = pow3(cur - 8);
        denominator -= 1;

        /* Previous cached value a(cur-1) is at cache[cur-9]  */
        seq_t val = cache[cur - 9] * numerator / denominator;

        /* Grow cache if necessary */
        if (cache_size <= cur - 8)
        {
            size_t new_size = cache_size ? cache_size * 2 : 8;
            while (new_size <= cur - 8)
                new_size *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp)
                return 0;  /* allocation failure – return 0 */
            cache = tmp;
            cache_size = new_size;
        }

        cache[cur - 8] = val;
    }

    cache_max = n;
    return cache[n - 8];
}
