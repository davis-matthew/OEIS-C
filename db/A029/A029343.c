/*
 * A029343  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A029343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coin–change DP for parts 4, 5, 9, 12: number of partitions of n into
   4, 5, 9, 12.  The function constructs a memoised table up to the
   largest argument requested.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of a(i) values          */
    static idx_t  computed_upto = 0;   /* highest index computed so far */

    /* If we need values beyond what we've already computed, rebuild */
    if (n > computed_upto || cache == NULL) {
        size_t new_cap = (size_t)(n + 1);   /* 0 … n inclusive   */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {      /* allocation failed – for safety return 0 */
            if (cache) free(cache);
            cache = NULL;
            computed_upto = 0;
            return (seq_t)0;
        }
        cache = tmp;

        /* initialise all entries to 0 */
        for (size_t i = 0; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;  /* one way to make 0 */

        /* coin–change DP: parts 4,5,9,12 */
        const idx_t coins[4] = {4, 5, 9, 12};
        for (int k = 0; k < 4; ++k) {
            idx_t coin = coins[k];
            for (idx_t i = coin; i <= n; ++i) {
                cache[i] = cache[i] + cache[i - coin];
            }
        }
        computed_upto = n;
    }

    return cache[n];
}
