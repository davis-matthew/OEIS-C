/*
 * A075129  Binomial transform of reflected tetranacci numbers A074058:
 *          a(n)=Sum((-1)^k Binomial(n,k)*A074058(k),(k=0,..,n)).
 * https://oeis.org/A075129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 3*a(n-4); a(0)=4,
 *            a(1)=5, a(2)=5, a(3)=5
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_len = 0;  /* number of computed entries */

    /* Allocate cache on first call */
    if (cache == NULL) {
        cache_size = (n < 4) ? 4 : n + 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0; /* allocation failed */
        cache[0] = 4;
        cache[1] = 5;
        cache[2] = 5;
        cache[3] = 5;
        cache_len = 4;
    }

    /* Grow cache if requested n is outside current size */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cache_size = n + 1;
    }

    /* If value already computed, return it */
    if (n < cache_len) return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term1 = 5 * cache[i - 1];
        seq_t term2 = 10 * cache[i - 3];
        seq_t pos = term1 + term2;

        seq_t term3 = 10 * cache[i - 2];
        seq_t term4 = 3 * cache[i - 4];
        seq_t neg = term3 + term4;

        cache[i] = pos - neg;  /* pos >= neg for all n >= 4 */
    }

    cache_len = n + 1;
    return cache[n];
}
