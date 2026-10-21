/*
 * A229456  Number of defective 3-colorings of an n X 5 0..2 array connected
 *          horizontally and vertically with exactly one mistake and colors
 *          introduced in row-major 0..2 order.
 * https://oeis.org/A229456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32*a(n-1) - 386*a(n-2) + 2264*a(n-3) - 7265*a(n-4)
 *            +13512*a(n-5) -14960*a(n-6) +9872*a(n-7) -3776*a(n-8)
 *            +768*a(n-9) -64*a(n-10) (n ≥ 11) Initial values: a(1)=16,
 *            a(2)=324, a(3)=4962, a(4)=68790, a(5)=903332, a(6)=11451686,
 *            a(7)=141595454, a(8)=1718447506, a(9)=20556781248,
 *            a(10)=243096369158
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1U)
        return (seq_t)0; /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* maximum n cached (index n) */
    idx_t old_max;

    /* Allocate and initialize cache for the first 10 terms if needed */
    if (cache_max == 0) {
        size_t init_cap = (size_t)(n > 10 ? n : 10);
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();

        cache[0]  = (seq_t)16ULL;
        cache[1]  = (seq_t)324ULL;
        cache[2]  = (seq_t)4962ULL;
        cache[3]  = (seq_t)68790ULL;
        cache[4]  = (seq_t)903332ULL;
        cache[5]  = (seq_t)11451686ULL;
        cache[6]  = (seq_t)141595454ULL;
        cache[7]  = (seq_t)1718447506ULL;
        cache[8]  = (seq_t)20556781248ULL;
        cache[9]  = (seq_t)243096369158ULL;
        cache_max = 10;
    }

    /* Grow cache if n exceeds current capacity */
    if (n > cache_max) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Return value if already cached */
    if (n <= cache_max)
        return cache[n - 1U];

    /* Compute missing terms up to n */
    old_max = cache_max;
    cache_max = n;   /* update after any possible reallocation */

    for (idx_t i = old_max + 1U; i <= n; ++i) {
        cache[i - 1U] =
            (seq_t)32    * cache[i - 2U]
          - (seq_t)386   * cache[i - 3U]
          + (seq_t)2264  * cache[i - 4U]
          - (seq_t)7265  * cache[i - 5U]
          + (seq_t)13512 * cache[i - 6U]
          - (seq_t)14960 * cache[i - 7U]
          + (seq_t)9872  * cache[i - 8U]
          - (seq_t)3776  * cache[i - 9U]
          + (seq_t)768   * cache[i -10U]
          - (seq_t)64    * cache[i -11U];
    }

    return cache[n - 1U];
}
