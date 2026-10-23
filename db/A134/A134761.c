/*
 * A134761  a(n) = (1/2)*( (1 + (-1)^n)*A134760(n/2) + (1 - (-1)^n) ).
 * https://oeis.org/A134761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 23, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A134761: a(n) = (1/2)*((1+(-1)^n)*A134760(n/2)+(1-(-1)^n)).
   For even n , a(2k)=A134760(k); for odd n , a(n)=1.
   A134760 satisfies the recurrence
      a(0)=a(1)=1 , a(2)=3
      a(n)= (-(n-1)*(3n-4)*a(n-1)+4*(3n²-10n+7)*a(n-2)+4*(n-2)*(3n-4)*a(n-3))
             / (n*(3n-7))      for n≥3
*/

#include <stdlib.h>

static seq_t *cache = NULL;       /* cache[k] = a(k) for A134761 */
static idx_t cache_size = 0;      /* current size of cache (number of stored values) */

static void ensure_cache(idx_t n)
{
    if ((idx_t)n + 1 <= cache_size)
        return;   /* already computed */

    /* Resize the cache */
    seq_t *tmp = (seq_t *)realloc(cache, ((idx_t)n + 1) * sizeof(*tmp));
    if (!tmp)
        abort();   /* out of memory */
    cache = tmp;

    /* Compute missing values sequentially */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i == 0)
            cache[0] = 1;
        else if (i == 1)
            cache[1] = 1;
        else if (i == 2)
            cache[2] = 3;
        else
        {
            seq_t nseq = (seq_t)i;

            /* term1 = (n-1)*(3n-4)*a(n-1) */
            seq_t term1 = (nseq - 1) * ((seq_t)3 * nseq - 4) * cache[i - 1];
            /* term2 = 4*(3n^2-10n+7)*a(n-2) */
            seq_t term2 = 4 * (3 * nseq * nseq - 10 * nseq + 7) * cache[i - 2];
            /* term3 = 4*(n-2)*(3n-4)*a(n-3) */
            seq_t term3 = 4 * (nseq - 2) * ((seq_t)3 * nseq - 4) * cache[i - 3];

            /* numerator = -term1 + term2 + term3  (all terms are non‑negative) */
            seq_t num = term2 + term3;
            num = num - term1;

            /* denominator = n*(3n-7) */
            seq_t denom = nseq * ((seq_t)3 * nseq - 7);

            cache[i] = num / denom;
        }
    }
    cache_size = (idx_t)n + 1;
}

seq_t a(idx_t n)
{
    /* Odd indices are always 1 */
    if (n & 1ULL)
        return 1;

    ensure_cache(n);
    return cache[n];
}
