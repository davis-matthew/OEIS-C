/*
 * A052770  Expansion of e.g.f. log(-1/(-1+x))^4*x.
 * https://oeis.org/A052770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 24 * n * s(n-1,4) where s(·,4) is the unsigned Stirling
 *            number of the first kind (number of permutations of {1..m} with
 *            exactly 4 cycles). The recurrence for s(m,k) is s(m,0)=0 (m>0),
 *            s(0,0)=1 s(m,k)=s(m-1,k-1)+(m-1)*s(m-1,k) For k=4 this gives an
 *            O(n) algorithm for all a(n)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache      = NULL;     /* cache of computed a-values */
    static size_t cache_cap  = 0;        /* allocated cache size   */
    static idx_t  max_n      = 0;        /* farthest index already filled */
    static seq_t s_prev[5];             /* s(m,k) for current m (k=0..4) */
    static seq_t s_new[5];              /* temporary storage for next step */
    static seq_t prev_s4  = 0;          /* s(m,4) for current m   */

    /* initialise on first call */
    if (!cache) {
        cache_cap = 1;           /* at least space for a(0) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;           /* a(0)=0 */
        /* initialise recurrence values for m=0   */
        s_prev[0] = 1;          /* s(0,0)=1 */
        s_prev[1] = s_prev[2] = s_prev[3] = s_prev[4] = 0;
        prev_s4 = 0;            /* s(0,4)=0 */
    }

    /* Expand cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = n * 2 + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();     /* fatal */
        cache = tmp;
        /* new entries are left uninitialized: they will be computed below */
        cache_cap = new_cap;
    }

    /* Compute all a-values up to index n */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        /* a(m) = 24 * m * s(m-1,4) */
        seq_t tmp = prev_s4 * (seq_t)24u;
        tmp = tmp * (seq_t)m;
        cache[m] = tmp;

        /* Update Stirling numbers for next step (for m+1) */
        s_new[0] = 0;                /* s(m+1,0)=0 for m+1>0 */
        for (int k = 1; k <= 4; ++k) {
            seq_t mul = s_prev[k] * ((seq_t)(m - 1));
            s_new[k] = s_prev[k - 1] + mul;
        }
        /* copy back */
        for (int k = 0; k <= 4; ++k) s_prev[k] = s_new[k];
        prev_s4 = s_prev[4];
    }

    max_n = n;
    return cache[n];
}
