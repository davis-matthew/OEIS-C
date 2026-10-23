/*
 * A167225  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=26, … computed by the recurrence derived from the
 *            rational generating function (t^14 + 2t^13 + … + 2t + 1) /
 *            (300t^14 – 24t^13 – … – 24t + 1) For n ≥ 0 define coeff(n) as 1
 *            if n==0 or n==14, 2 if 1 ≤ n ≤ 13, 0 otherwise. Then a(n) =
 *            24·(a(n-1)+…+a(n-13)) – 300·a(n-14) + coeff(n) (wit...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised values */
static idx_t cache_len = 0;   /* number of computed entries (next to fill) */

static void ensure_cache(idx_t n)
{
    if (n < cache_len)            /* already computed */
        return;

    /* enlarge the array to hold indices up to n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp)
        abort();                  /* allocation failed */
    cache = tmp;

    /* fill missing values */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t sum = 0;

        /* 24·(a(i-1)+…+a(i-13)) */
        for (idx_t k = 1; k <= 13; ++k)
        {
            if (i >= k)
                sum += cache[i - k] * 24;
        }

        /* – 300·a(i-14) */
        if (i >= 14)
            sum -= cache[i - 14] * 300;

        /* constant term from the numerator */
        seq_t coeff = 0;
        if (i == 0 || i == 14)
            coeff = 1;
        else if (i <= 13)
            coeff = 2;

        sum += coeff;
        cache[i] = sum;
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
