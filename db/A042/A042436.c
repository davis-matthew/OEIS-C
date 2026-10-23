/*
 * A042436  Numerators of continued fraction convergents to sqrt(746).
 * https://oeis.org/A042436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=27. Let c_n be the periodic coefficients [3, 5, 7, 1, 1, 1,
 *            1, 1, 1, 7, 5, 3, 54] (length 13) for n ≥ 1. Then a(n) = c_n *
 *            a(n-1) + a(n-2) for n ≥ 1. The coefficients repeat every 13
 *            terms after the initial 27
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 14, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Periodic coefficients of the continued fraction of sqrt(746). */
    static const seq_t period_coefs[] = {
        3,5,7,1,1,1,1,1,1,7,5,3,54
    };
    const idx_t period_len = sizeof(period_coefs)/sizeof(period_coefs[0]);

    /* Cache of previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* If value already computed, return it. */
    if (n < cache_len)
        return cache[n];

    /* Ensure the cache is large enough. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    }
    if (!cache) {  /* allocation failure – not expected in test harness */
        return 0;
    }

    /* Initialize the recurrence variables from the cache. */
    seq_t prev2, prev1; /* p_{i-1} and p_{i-2} */
    if (cache_len == 0) {
        prev2 = 0;         /* p_-2 */
        prev1 = 1;         /* p_-1 */
    } else if (cache_len == 1) {
        prev2 = 1;         /* p_-1 */
        prev1 = cache[0];  /* p_0 = 27 */
    } else {
        prev2 = cache[cache_len - 2];
        prev1 = cache[cache_len - 1];
    }

    /* Compute terms up to index n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t coeff = (i == 0 ? (seq_t)27
                   : period_coefs[(i - 1) % period_len]);
        seq_t curr = coeff * prev1 + prev2;
        cache[i] = curr;
        prev2 = prev1;
        prev1 = curr;
    }

    cache_len = n + 1;
    return cache[n];
}
