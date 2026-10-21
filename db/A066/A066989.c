/*
 * A066989  a(n) = (n!)^3 * Sum_{i=1..n} 1/i^3.
 * https://oeis.org/A066989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 9, a(n+2) = (2n+3)*(n^2+3n+3)*a(n+1) -
 *            (n+1)^6*a(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A066989: a(n) = (n!)^3 * Σ_{i=1..n} 1/i^3   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 0‑indexed cache: cache[0] unused, cache[1] = a(1), etc.  */
    static seq_t *cache = NULL;
    static idx_t  max_cached = 0;      /* largest n computed so far   */
    static size_t cache_size = 0;      /* capacity of cache array      */

    if (n == 0) /* offset starts at 1, return 0 for safety */
        return (seq_t)0;

    /* ensure cache is large enough */
    if ((size_t)(n + 1) > cache_size) {
        size_t needed = (size_t)(n + 1);
        if (needed < 3) needed = 3;        /* at least space for indices 1 and 2 */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)                      /* allocation failed → undefined behaviour */
            return (seq_t)0;
        cache = tmp;
        cache_size = needed;
    }

    /* initialise base cases if necessary */
    if (max_cached < 1 && n >= 1) {
        cache[1] = (seq_t)1;
        max_cached = 1;
    }
    if (max_cached < 2 && n >= 2) {
        cache[2] = (seq_t)9;
        max_cached = 2;
    }

    /* compute subsequent values using the recurrence */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        idx_t p = k - 2;   /* n in recurrence for a(k) */
        idx_t m = k - 1;   /* (k-1) for the second term */

        /* coefficient c1 = (2p+3)*(p^2 + 3p + 3) */
        seq_t c1 = ((seq_t)(2 * p + 3)) *
                   ((seq_t)(p * p + 3 * p + 3));

        /* coefficient c2 = (m+1)^6  ->  (m)^6 for recurrence step */
        /* but in recurrence we need (k-1)^6 = m^6 */
        seq_t m2 = (seq_t)(m * m);
        seq_t m3 = m2 * (seq_t)m;
        seq_t c2 = m3 * m3;   /* (m)^6 */

        cache[k] = c1 * cache[k - 1] - c2 * cache[k - 2];
    }

    max_cached = n;
    return cache[n];
}
