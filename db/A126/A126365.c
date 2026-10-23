/*
 * A126365  Number of base 11 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) counts base‑11 n‑digit strings with adjacent digits
 *            differing by at most 1. Let w_k(i) be the number of such strings
 *            of length k ending with digit i (0≤i≤10). Initial state:
 *            w_1(i)=1 for all i. Recurrence:
 *            w_{k+1}(i)=w_k(i)+w_k(i-1)+w_k(i+1) (missing terms omitted).
 *            Then a(n)=∑_{i=0}^{10} w_...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* a(0), a(1), … */
    static idx_t  cache_size = 0;        /* number of elements stored in cache */
    static seq_t  dp[11];                /* counts for current length */
    static seq_t  dp_next[11];           /* temporary for next length */

    /* handle a(0) immediately */
    if (n == (idx_t)0) return (seq_t)1;

    /* first call: allocate cache and initialise dp for length 1 */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);   /* indices 0 and 1 */
        if (!cache) return (seq_t)0;                  /* allocation failure */
        cache[0] = (seq_t)1;                          /* a(0) */

        /* dp for length 1: each digit occurs once */
        for (int i = 0; i < 11; ++i) dp[i] = (seq_t)1;

        /* compute a(1) = sum(dp) = 11 */
        seq_t total = 0;
        for (int i = 0; i < 11; ++i) total += dp[i];
        cache[1] = total;
        cache_size = 2;
    }

    /* extend cache up to requested n (if necessary) */
    while (cache_size <= n) {
        /* compute dp_next for length cache_size (currently dp represents length cache_size-1) */
        for (int i = 0; i < 11; ++i) {
            dp_next[i] = dp[i];
            if (i > 0)      dp_next[i] += dp[i - 1];
            if (i < 10)     dp_next[i] += dp[i + 1];
        }

        /* total number of strings of this length */
        seq_t total = 0;
        for (int i = 0; i < 11; ++i) total += dp_next[i];

        /* enlarge cache to store this term */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (cache_size + 1));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache[cache_size] = total;
        ++cache_size;

        /* move to next dp */
        for (int i = 0; i < 11; ++i) dp[i] = dp_next[i];
    }

    return cache[n];
}
