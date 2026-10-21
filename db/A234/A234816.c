/*
 * A234816  Number of (n+1) X (1+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 6
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 41*a(n-2) - 164*a(n-3) - 558*a(n-4) +
 *            2232*a(n-5) + 2520*a(n-6) - 10080*a(n-7). Initial values:
 *            a(1)=168, a(2)=686, a(3)=2380, a(4)=9716, a(5)=34092,
 *            a(6)=139112, a(7)=493348
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values, 1‑based, cache[0] unused */
static size_t cache_cap = 0;     /* allocated capacity of cache array */
static idx_t cache_upto = 0;     /* largest n that has been computed */


/* Ensure that cache has room for indices up to 'n' inclusive. */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 8;                 /* we will store at least indices 0..7 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;           /* unused */
        cache[1] = (seq_t)168;
        cache[2] = (seq_t)686;
        cache[3] = (seq_t)2380;
        cache[4] = (seq_t)9716;
        cache[5] = (seq_t)34092;
        cache[6] = (seq_t)139112;
        cache[7] = (seq_t)493348;
        cache_upto = 7;
    }

    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}


seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */

    ensure_capacity(n);

    if (n <= cache_upto) return cache[n];

    /* compute values from cache_upto+1 up to n */
    for (idx_t i = cache_upto + 1; i <= n; ++i) {
        size_t idx = (size_t)i;
        seq_t val = cache[i-1] * (seq_t)4
                  + cache[i-2] * (seq_t)41
                  - cache[i-3] * (seq_t)164
                  - cache[i-4] * (seq_t)558
                  + cache[i-5] * (seq_t)2232
                  + cache[i-6] * (seq_t)2520
                  - cache[i-7] * (seq_t)10080;
        cache[idx] = val;
    }
    cache_upto = n;
    return cache[(size_t)n];
}
