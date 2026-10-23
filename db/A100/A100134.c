/*
 * A100134  a(n) = Sum_{k=0..floor(n/6)} binomial(n-3k,3k).
 * https://oeis.org/A100134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/6)} binomial(n-3k,3k) Recurrence:
 *            a(n)=3*a(n-1)-3*a(n-2)+a(n-3)+a(n-6), a(0..5)=1
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 49, u64 n <= 95, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cacheSize = 0;   /* number of elements allocated */

    if (!cache) {
        /* first call: allocate space for n+1 values */
        cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* Allocation failed; return 0 */
        cacheSize = n + 1;
    } else if (n + 1 > cacheSize) {
        /* need more space */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* Allocation failed; return 0 */
        cache = tmp;
        cacheSize = n + 1;
    }

    /* Ensure the base values a(0)..a(5) are set to 1 */
    for (idx_t i = 0; i <= 5 && i < cacheSize; ++i)
        cache[i] = (seq_t)1;

    /* Compute missing values up to index n */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t term1 = (seq_t)3 * cache[i - 1];
        term1 += cache[i - 3];
        term1 += cache[i - 6];
        seq_t term2 = (seq_t)3 * cache[i - 2];
        cache[i] = term1 - term2;
    }

    return cache[n];
}
