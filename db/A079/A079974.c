/*
 * A079974  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=4, I={0,2}.
 * https://oeis.org/A079974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-4)+a(n-5); initial values: a(0)=1, a(1)=0,
 *            a(2)=1, a(3)=0, a(4)=2, a(5)=1
 * Range:     u8 n <= 20, u16 n <= 37, u32 n <= 72, u64 n <= 141,
 *            u128 n <= 278, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of compositions of n into 2, 4, and 5. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* Initialize base values on first call. */
    if (cache_cap == 0) {
        cache_cap = 8;               /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;        /* should not happen */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)1;
        cache_len = 6;               /* next index to compute is 6 */
    }

    /* Expand cache if necessary. */
    if ((idx_t)n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap <= (idx_t)n)
            new_cap = (idx_t)n + 1;   /* at least n+1 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return stored value if already computed. */
    if (n < cache_len)
        return cache[n];

    /* Compute missing values up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 2) val += cache[i - 2];
        if (i >= 4) val += cache[i - 4];
        if (i >= 5) val += cache[i - 5];
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
