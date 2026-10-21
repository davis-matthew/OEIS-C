/*
 * A233921  Number of (n+1) X (1+1) 0..4 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of the edge differences
 *          equal to 8 and no adjacent elements equal.
 * https://oeis.org/A233921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 5*a(n-2) – 46*a(n-3) + 5*a(n-4) + 60*a(n-5) +
 *            20*a(n-6) Initial values: a(1)=98, a(2)=518, a(3)=2792,
 *            a(4)=15230, a(5)=83290, a(6)=456328
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow out‑of‑order calls */
    static seq_t *cache = NULL;          /* array of computed values 1..len */
    static idx_t cache_len = 0;          /* number of values stored */
    static idx_t cache_cap = 0;          /* allocated capacity (elements) */

    /* handle offset 1: a(1) is the first term */
    if (n == 0)
        return (seq_t)0;

    /* initialise cache with the first 6 known values once */
    if (cache == NULL) {
        cache_cap = 6;
        cache_len = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)98;      /* a(1) */
        cache[1] = (seq_t)518;     /* a(2) */
        cache[2] = (seq_t)2792;    /* a(3) */
        cache[3] = (seq_t)15230;   /* a(4) */
        cache[4] = (seq_t)83290;   /* a(5) */
        cache[5] = (seq_t)456328;  /* a(6) */
    }

    /* returns cached value if already computed */
    if (n <= cache_len)
        return cache[n-1];

    /* expand cache to accommodate up to n */
    if (n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = n;
    }

    /* compute subsequently using the empirical recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        /* indices: i-1 = n-1, i-2 = n-2, ..., i-6 = n-6 */
        seq_t pos = (seq_t)6 * cache[i-1];
        pos += (seq_t)5 * cache[i-2];
        pos += (seq_t)5 * cache[i-4];
        pos += (seq_t)60 * cache[i-5];
        pos += (seq_t)20 * cache[i-6];
        seq_t neg = (seq_t)46 * cache[i-3];
        cache[i] = pos - neg;
    }

    cache_len = n;
    return cache[n-1];
}
