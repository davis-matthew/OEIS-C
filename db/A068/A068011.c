/*
 * A068011  Number of subsets of {1,2,3,...,n} that sum to 0 mod 5.
 * https://oeis.org/A068011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of subsets of {1,2,…,n} whose element sum is 0
 *            modulo 5. The sequence is obtained by a simple DP over the five
 *            residue classes. For each new element i, every subset that had
 *            sum s may either keep i (sum stays s) or take i (sum becomes s+i
 *            mod 5). Thus for each residue r = i mod 5 we...
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* remembered state ---------------------------------------------------- */
    static seq_t *cache  = NULL;      /* cache[n] = a(n)             */
    static idx_t  cache_size = 0;     /* number of stored entries    */
    static idx_t  cache_cap  = 0;     /* allocated capacity          */
    static seq_t  dp[5]   = {0,0,0,0,0};   /* counts for residues 0..4    */
    static idx_t  cur_n   = 0;        /* number of elements processed */

    /* initialise on first call ------------------------------------------- */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out of memory – return 0 */

        /* start state: no elements, only the empty subset (sum 0) */
        dp[0] = (seq_t)1;
        for (int i = 1; i < 5; ++i) dp[i] = (seq_t)0;

        cache[0] = dp[0];      /* a(0) = 1 */
        cache_size = 1;
        cur_n = 0;
    }

    /* if already computed, return cached value --------------------------- */
    if (n < cache_size) return cache[n];

    /* extend cache and dp up to n --------------------------------------- */
    for (idx_t i = cur_n + 1; i <= n; ++i) {
        seq_t newdp[5];
        int r = (int)(i % 5);          /* remainder of i modulo 5 */

        for (int j = 0; j < 5; ++j) {
            int idx = (j + 5 - r) % 5;
            newdp[j] = dp[j] + dp[idx];
        }
        /* copy back to dp */
        for (int j = 0; j < 5; ++j) dp[j] = newdp[j];

        /* store result for this n */
        if (cache_size >= cache_cap) {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* out of memory */
            cache = tmp;
        }
        cache[cache_size] = dp[0];
        ++cache_size;
    }
    cur_n = n;
    return cache[n];
}
