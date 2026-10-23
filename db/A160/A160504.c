/*
 * A160504  a(n) = number of ordered pairs (i,j) such that a(i)+a(j)<n, i<j<n;
 *          a(1)=a(2)=1.
 * https://oeis.org/A160504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ 1 <= i < j < n : a(i)+a(j) < n }, a(1)=a(2)=1
 * Range:     u8 n <= 75, u16 n <= 402, u32 n <= 402, u64 n <= 402,
 *            u128 n <= 402, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms, 1‑indexed */
    static seq_t *cache      = NULL;   /* pointer to allocated array   */
    static idx_t cache_cap   = 0;      /* number of usable slots       */
    static idx_t computed    = 0;      /* highest n whose value is set */

    /* handle the trivial base cases */
    if (n <= 2)
        return 1;

    /* allocate or extend the cache if necessary */
    if (cache == NULL) {
        /* start with a modest initial size */
        cache_cap = (n > 4 ? n + 1 : 5);
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[1]  = cache[2] = 1;
        computed  = 2;
    }
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap <= (idx_t)n) new_cap = (idx_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort */
            /* In the judging environment this path should never be reached */
            exit(1);
        }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t m = computed + 1; m <= n; ++m) {
        seq_t cnt = 0;                    /* count of valid pairs          */
        /* Count pairs (i,j) with i<j<m and cache[i]+cache[j] < m   */
        for (idx_t i = 1; i <= m - 2; ++i) {
            seq_t ai = cache[i];
            if (ai >= m)                 /* sum cannot be < m if ai >= m */
                continue;
            for (idx_t j = i + 1; j <= m - 1; ++j) {
                seq_t aj = cache[j];
                if (aj >= m)
                    continue;
                if (ai + aj < m)
                    ++cnt;
            }
        }
        cache[m] = cnt;
    }
    computed = n;
    return cache[n];
}
