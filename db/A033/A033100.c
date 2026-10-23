/*
 * A033100  Number of 3's when k is written in base b for all b and k
 *          satisfying 2<=b<=n+1, 1<=k<=n.
 * https://oeis.org/A033100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = \sum_{k=1}^{n}\;\sum_{b=2}^{k+1} \text{count}_3(k \text{
 *            in base }b)
 * Range:     u8 n <= 53, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* each inner count is the number of times digit '3' appears when k is written in base b  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* out of range values are simply 0   */
        return 0;

    static seq_t *cache = NULL;      /* memoised cumulative sums a(0..i)          */
    static idx_t cache_upto = 0;     /* highest index already computed          */

    if (cache_upto < n)             /* extend the cache up to the requested n */
    {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)            /* allocation failure – terminate neatly   */
            exit(1);
        cache = new_cache;

        if (cache_upto == 0)       /* initialise a(0) */
            cache[0] = 0;

        for (idx_t k = cache_upto + 1; k <= n; ++k)
        {
            seq_t add = 0;         /* f(k) – contribution of this k            */

            /* for this k we need bases 2 .. k+1 */
            for (idx_t b = 2; b <= k + 1; ++b)
            {
                idx_t tmp = k;
                while (tmp)
                {
                    if (tmp % b == 3)
                        ++add;
                    tmp /= b;
                }
            }
            cache[k] = cache[k - 1] + add;   /* a(k) = a(k-1)+add               */
        }
        cache_upto = n;
    }
    return cache[n];
}
