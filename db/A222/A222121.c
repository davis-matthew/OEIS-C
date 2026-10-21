/*
 * A222121  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..2 array extended with zeros and
 *          convolved with 1,2,1.
 * https://oeis.org/A222121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-6) + a(n-8) for n ≥ 9 Initial terms
 *            (offset 1): a(1)=2, a(2)=3, a(3)=4, a(4)=6, a(5)=10, a(6)=15,
 *            a(7)=24, a(8)=38
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 49, u64 n <= 99, u128 n <= 198,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache  (offset 1: cache[i] == a(i+1)) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;   /* maximum n computed so far */

    /* Base values for n = 1..8 */
    static const seq_t base[8] = {2,3,4,6,10,15,24,38};

    /* Initialise cache on first call */
    if (cache_len == 0) {
        /* Allocate space for at least 8 terms */
        cache_cap = 8;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure → 0 */

        /* Store the first eight terms */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = base[i];
        cache_len = 8;
    }

    /* If the requested n is already known, return it */
    if (n <= (idx_t)cache_len)
        return cache[n - 1];

    /* Grow the cache if necessary */
    if (n > (idx_t)cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failure → 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all terms up to n */
    for (idx_t k = cache_len + 1; k <= n; ++k)
    {
        seq_t val = 0;
        if (k > 1)  val += cache[k - 2];       /* a(k-1) */
        if (k > 3)  val += cache[k - 4];       /* a(k-3) */
        if (k > 6)  val += cache[k - 7];       /* a(k-6) */
        if (k > 8)  val += cache[k - 9];       /* a(k-8) */
        cache[k - 1] = val;
    }
    cache_len = n;
    return cache[n - 1];
}
